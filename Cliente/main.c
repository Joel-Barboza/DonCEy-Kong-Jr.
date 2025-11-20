#define SDL_MAIN_HANDLED
#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>
#include "socket_thread.h"
#include <stdio.h>
#include <string.h>
#include "cjson/cJSON.h"
#include "widgets/button.h"
#include "widgets/input_field.h"

#ifdef _WIN32
    #include <winsock2.h>
    #include <ws2tcpip.h>
    #pragma comment(lib, "ws2_32.lib")
    #define close closesocket
    #define read(s, b, l) recv(s, b, l, 0)
    #define write(s, b, l) send(s, b, l, 0)
#else
    #include <unistd.h>
    #include <sys/socket.h>
    #include <netinet/in.h>
    #include <arpa/inet.h>
#endif

#define PORT 8080
#define BUFFER_SIZE 1024
extern int connected;


// -----------------------------
// Nivel: plataformas y lianas
// -----------------------------
#define NUM_PLATFORMS 10
#define NUM_VINES 9

SDL_FRect platforms[NUM_PLATFORMS];
SDL_FRect vines[NUM_VINES];

// -----------------------------
// Mono (personaje)
// -----------------------------
typedef struct {
    SDL_FRect rect;
    float velocityX;
    float velocityY;
    int isOnGround;
    int facingRight;
    int isOnVine;        // Nuevo: indica si está en una liana
    int currentVine;     // Nuevo: índice de la liana actual
    float vineOffsetX;   // Nuevo: offset horizontal en la liana
} Monkey;

Monkey monkey;

// Constantes de física
const float GRAVITY = 0.5f;
const float JUMP_FORCE = -10.0f;
const float MOVE_SPEED = 3.0f;
const float CLIMB_SPEED = 2.5f;  // Nuevo: velocidad de escalada

void initialize_level_elements() {
    platforms[0] = (SDL_FRect){ 20.0f,  60.0f, 400.0f, 18.0f };
    platforms[1] = (SDL_FRect){ 400.0f,  80.0f, 250.0f, 18.0f };
    platforms[2] = (SDL_FRect){ 600.0f,  300.0f, 250.0f, 18.0f };
    platforms[3] = (SDL_FRect){ 200.0f,  200.0f, 100.0f, 18.0f };
    platforms[4] = (SDL_FRect){ 200.0f,  310.0f, 200.0f, 18.0f };
    platforms[5] = (SDL_FRect){ 20.0f,  490.0f, 200.0f, 18.0f };
    platforms[6] = (SDL_FRect){ 380.0f,  430.0f, 100.0f, 18.0f };
    platforms[7] = (SDL_FRect){ 510.0f,  460.0f, 100.0f, 18.0f };
    platforms[8] = (SDL_FRect){ 670.0f,  440.0f, 100.0f, 18.0f };

    vines[0]  = (SDL_FRect){  680.0f,  30.0f, 6.0f,  380.0f };
    vines[1]  = (SDL_FRect){ 750.0f,  30.0f, 6.0f,  380.0f };
    vines[2]  = (SDL_FRect){ 280.0f,  200.0f, 6.0f,  200.0f };
    vines[3]  = (SDL_FRect){  80.0f,  78.0f, 6.0f,  300.0f };
    vines[4]  = (SDL_FRect){ 150.0f,  78.0f, 6.0f,  300.0f };
    vines[5]  = (SDL_FRect){ 430.0f,  98.0f, 6.0f,  300.0f };
    vines[6]  = (SDL_FRect){ 480.0f,  98.0f, 6.0f,  200.0f };
    vines[7]  = (SDL_FRect){ 590.0f,  98.0f, 6.0f,  260.0f };
    vines[8]  = (SDL_FRect){ 530.0f,  98.0f, 6.0f,  300.0f };
}

void initialize_monkey() {
    // Posición inicial del mono (sobre la primera plataforma)
    monkey.rect = (SDL_FRect){ 50.0f, 470.0f, 24.0f, 32.0f };
    monkey.velocityX = 0.0f;
    monkey.velocityY = 0.0f;
    monkey.isOnGround = 0;
    monkey.facingRight = 1;
    monkey.isOnVine = 0;      // Inicialmente no está en liana
    monkey.currentVine = -1;  // Sin liana actual
    monkey.vineOffsetX = 0.0f;
}

// Función para verificar si el mono puede agarrarse a una liana
int check_vine_collision() {
    for (int i = 0; i < NUM_VINES; i++) {
        SDL_FRect v = vines[i];

        // Verificar si el mono está tocando la liana
        if (monkey.rect.x < v.x + v.w &&
            monkey.rect.x + monkey.rect.w > v.x &&
            monkey.rect.y < v.y + v.h &&
            monkey.rect.y + monkey.rect.h > v.y) {

            // El mono está tocando una liana
            return i;
        }
    }
    return -1; // No está tocando ninguna liana
}

// Sistema de colisiones mejorado con soporte para lianas
void update_monkey_physics() {
    // Si está en una liana, manejar movimiento especial
    if (monkey.isOnVine) {
        // Mantener al mono en la posición de la liana
        SDL_FRect currentVine = vines[monkey.currentVine];
        monkey.rect.x = currentVine.x + monkey.vineOffsetX;

        // Aplicar movimiento vertical (escalada)
        monkey.rect.y += monkey.velocityY;

        // Verificar límites de la liana
        if (monkey.rect.y < currentVine.y) {
            monkey.rect.y = currentVine.y;
            monkey.velocityY = 0;
        }
        if (monkey.rect.y + monkey.rect.h > currentVine.y + currentVine.h) {
            monkey.rect.y = currentVine.y + currentVine.h - monkey.rect.h;
            monkey.velocityY = 0;

            // Si llega al fondo de la liana, verificar si está sobre una plataforma
            for (int i = 0; i < NUM_PLATFORMS; i++) {
                if (monkey.rect.x < platforms[i].x + platforms[i].w &&
                    monkey.rect.x + monkey.rect.w > platforms[i].x &&
                    monkey.rect.y + monkey.rect.h <= platforms[i].y + 5 &&  // Cerca de la plataforma
                    monkey.rect.y + monkey.rect.h >= platforms[i].y) {

                    // Salir de la liana y colocarse sobre la plataforma
                    monkey.isOnVine = 0;
                    monkey.currentVine = -1;
                    monkey.rect.y = platforms[i].y - monkey.rect.h;
                    monkey.velocityY = 0;
                    monkey.isOnGround = 1;
                    return;
                }
            }
        }

        return; // Saltar el resto de la física normal cuando está en liana
    }

    // Física normal (cuando no está en liana)

    // Aplicar gravedad
    monkey.velocityY += GRAVITY;

    // Mover en X
    monkey.rect.x += monkey.velocityX;

    // Verificar colisiones horizontales con plataformas
    for (int i = 0; i < NUM_PLATFORMS; i++) {
        if (monkey.rect.x < platforms[i].x + platforms[i].w &&
            monkey.rect.x + monkey.rect.w > platforms[i].x &&
            monkey.rect.y < platforms[i].y + platforms[i].h &&
            monkey.rect.y + monkey.rect.h > platforms[i].y) {

            if (monkey.velocityX > 0) {
                // Colisión por la derecha
                monkey.rect.x = platforms[i].x - monkey.rect.w;
            } else if (monkey.velocityX < 0) {
                // Colisión por la izquierda
                monkey.rect.x = platforms[i].x + platforms[i].w;
            }
            monkey.velocityX = 0;
        }
    }

    // Mover en Y
    monkey.rect.y += monkey.velocityY;
    monkey.isOnGround = 0;

    // Verificar colisiones verticales con plataformas
    for (int i = 0; i < NUM_PLATFORMS; i++) {
        if (monkey.rect.x < platforms[i].x + platforms[i].w &&
            monkey.rect.x + monkey.rect.w > platforms[i].x &&
            monkey.rect.y < platforms[i].y + platforms[i].h &&
            monkey.rect.y + monkey.rect.h > platforms[i].y) {

            if (monkey.velocityY > 0) {
                // Colisión por abajo (cayendo sobre plataforma)
                monkey.rect.y = platforms[i].y - monkey.rect.h;
                monkey.velocityY = 0;
                monkey.isOnGround = 1;
            } else if (monkey.velocityY < 0) {
                // Colisión por arriba (saltando contra plataforma)
                monkey.rect.y = platforms[i].y + platforms[i].h;
                monkey.velocityY = 0;
            }
        }
    }

    // Limites de pantalla
    if (monkey.rect.x < 0) {
        monkey.rect.x = 0;
        monkey.velocityX = 0;
    }
    if (monkey.rect.x + monkey.rect.w > 800) {
        monkey.rect.x = 800 - monkey.rect.w;
        monkey.velocityX = 0;
    }
    if (monkey.rect.y + monkey.rect.h > 600) {
        monkey.rect.y = 600 - monkey.rect.h;
        monkey.velocityY = 0;
        monkey.isOnGround = 1;
    }

    // Fricción simple
    if (monkey.isOnGround) {
        monkey.velocityX *= 0.8f;
        if (SDL_fabsf(monkey.velocityX) < 0.5f) monkey.velocityX = 0;
    }
}

void handle_input(const _Bool* keyboard_state) {
    // Si está en una liana, manejar controles de escalada
    if (monkey.isOnVine) {
        monkey.velocityX = 0; // No movimiento horizontal en lianas

        // Escalada hacia arriba y abajo
        if (keyboard_state[SDL_SCANCODE_UP]) {
            monkey.velocityY = -CLIMB_SPEED;
        }
        else if (keyboard_state[SDL_SCANCODE_DOWN]) {
            monkey.velocityY = CLIMB_SPEED;
        }
        else {
            monkey.velocityY = 0; // No moverse si no se presiona nada
        }

        // Salto para soltarse de la liana
        if (keyboard_state[SDL_SCANCODE_LEFT] || keyboard_state[SDL_SCANCODE_RIGHT]) {
            monkey.isOnVine = 0;
            monkey.currentVine = -1;
            monkey.velocityY = 0;
            // Dar un pequeño impulso en la dirección presionada
            if (keyboard_state[SDL_SCANCODE_LEFT]) {
                monkey.velocityX = -MOVE_SPEED * 0.5f;
                monkey.facingRight = 0;
            } else {
                monkey.velocityX = MOVE_SPEED * 0.5f;
                monkey.facingRight = 1;
            }
        }

        return;
    }

    // Movimiento normal (cuando no está en liana)

    // Movimiento horizontal
    if (keyboard_state[SDL_SCANCODE_LEFT]) {
        monkey.velocityX = -MOVE_SPEED;
        monkey.facingRight = 0;
    }
    else if (keyboard_state[SDL_SCANCODE_RIGHT]) {
        monkey.velocityX = MOVE_SPEED;
        monkey.facingRight = 1;
    }

    // Salto
    if (keyboard_state[SDL_SCANCODE_UP] && monkey.isOnGround) {
        monkey.velocityY = JUMP_FORCE;
        monkey.isOnGround = 0;
    }

    // Verificar si puede agarrarse a una liana (cuando salta cerca de una)
    if (!monkey.isOnGround && keyboard_state[SDL_SCANCODE_UP]) {
        int vineIndex = check_vine_collision();
        if (vineIndex != -1) {
            monkey.isOnVine = 1;
            monkey.currentVine = vineIndex;
            monkey.velocityY = 0;
            monkey.velocityX = 0;
            // Calcular offset para centrarse en la liana
            monkey.vineOffsetX = (vines[vineIndex].w - monkey.rect.w) / 2.0f;
        }
    }
}

void draw_monkey(SDL_Renderer* renderer) {
    // Si está en liana, cambiar color para indicarlo
    if (monkey.isOnVine) {
        SDL_SetRenderDrawColor(renderer, 100, 50, 10, 255); // Marrón más oscuro
    } else {
        SDL_SetRenderDrawColor(renderer, 139, 69, 19, 255); // Marrón normal
    }
    SDL_RenderFillRect(renderer, &monkey.rect);

    // Cara
    SDL_SetRenderDrawColor(renderer, 255, 220, 170, 255);
    SDL_FRect face = {
        monkey.rect.x + (monkey.facingRight ? 6 : 2),
        monkey.rect.y + 4,
        monkey.rect.w - 8,
        monkey.rect.h - 12
    };
    SDL_RenderFillRect(renderer, &face);

    // Ojos
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    if (monkey.facingRight) {
        SDL_FRect eye1 = { monkey.rect.x + 14, monkey.rect.y + 10, 3, 3 };
        SDL_FRect eye2 = { monkey.rect.x + 18, monkey.rect.y + 10, 3, 3 };
        SDL_RenderFillRect(renderer, &eye1);
        SDL_RenderFillRect(renderer, &eye2);
    } else {
        SDL_FRect eye1 = { monkey.rect.x + 7, monkey.rect.y + 10, 3, 3 };
        SDL_FRect eye2 = { monkey.rect.x + 11, monkey.rect.y + 10, 3, 3 };
        SDL_RenderFillRect(renderer, &eye1);
        SDL_RenderFillRect(renderer, &eye2);
    }
}

// Dibuja una "textura" simple de plataforma estilo arcade (base + borde)
void draw_platform_rect(SDL_Renderer* renderer, SDL_FRect r) {
    // base marrón
    SDL_SetRenderDrawColor(renderer, 150, 70, 30, 255);
    SDL_RenderFillRect(renderer, &r);

    // borde superior (línea brillante)
    SDL_SetRenderDrawColor(renderer, 250, 150, 60, 255);
    SDL_FRect top = { r.x, r.y, r.w, 4 };
    SDL_RenderFillRect(renderer, &top);

    // líneas horizontales decorativas (simular ladrillos)
    SDL_SetRenderDrawColor(renderer, 170, 90, 40, 255);
    float brick_h = 4.0f;
    for (float bx = r.x + 6.0f; bx + 20.0f < r.x + r.w; bx += 26.0f) {
        SDL_FRect brick = { bx, r.y + 6.0f, 20.0f, brick_h };
        SDL_RenderFillRect(renderer, &brick);
    }
}

void draw_platforms(SDL_Renderer* renderer) {
    for (int i = 0; i < NUM_PLATFORMS; ++i) {
        draw_platform_rect(renderer, platforms[i]);
    }
}

void draw_vines(SDL_Renderer* renderer) {
    for (int i = 0; i < NUM_VINES; ++i) {
        SDL_FRect v = vines[i];

        // Si el mono está en esta liana, resaltarla
        if (monkey.isOnVine && monkey.currentVine == i) {
            SDL_SetRenderDrawColor(renderer, 120, 230, 100, 255); // Verde brillante
        } else {
            SDL_SetRenderDrawColor(renderer, 90, 200, 80, 255); // Verde normal
        }
        SDL_RenderFillRect(renderer, &v);

        // nudos / hojas cada 28 px a partir del inicio
        SDL_SetRenderDrawColor(renderer, 20, 120, 20, 255);
        float knot_step = 28.0f;
        for (float y = v.y + 10.0f; y < v.y + v.h; y += knot_step) {
            SDL_FRect knot = { v.x - 5.0f, y, v.w + 10.0f, 4.0f };
            SDL_RenderFillRect(renderer, &knot);
        }

        if (v.h > 200.0f) {
            SDL_SetRenderDrawColor(renderer, 30, 160, 40, 255);
            SDL_FRect leaf = { v.x - 8.0f, v.y + v.h - 8.0f, v.w + 16.0f, 6.0f };
            SDL_RenderFillRect(renderer, &leaf);
        }
    }
}


void draw_game (SDL_Renderer *renderer, const _Bool* keyboard_state) {
    // Manejar entrada del teclado
    handle_input(keyboard_state);

    // Actualizar física del mono
    update_monkey_physics();

    // Fondo negro
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    // Dibujar elementos del nivel
    draw_platforms(renderer);
    draw_vines(renderer);

    // Dibujar el mono
    draw_monkey(renderer);

    // Dibujar botón
    // draw_button(renderer, &btn);

    SDL_RenderPresent(renderer);

    SDL_Delay(16); // ~60 FPS
}

void draw_connect (SDL_Renderer *renderer, Button *btn, InputField *input_field) {

    draw_input_field(renderer, input_field);
    draw_button(renderer, btn);
    SDL_RenderPresent(renderer);
}

// ---------------------------
// Ventana y loop principal
// ---------------------------
int window() {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        SDL_Log("SDL init failed: %s", SDL_GetError());
        return -1;
    }

    if (TTF_Init() < 0) {
        SDL_Log("TTF init failed: %s", SDL_GetError());
        SDL_Quit();
        return -1;
    }

    SDL_Window *window = SDL_CreateWindow("Donkey Kong Jr", 800, 600, 0);
    if (!window) {
        SDL_Log("Window create failed: %s", SDL_GetError());
        TTF_Quit();
        SDL_Quit();
        return -1;
    }

    SDL_Renderer *renderer = SDL_CreateRenderer(window, NULL);
    if (!renderer) {
        SDL_Log("Renderer create failed: %s", SDL_GetError());
        SDL_DestroyWindow(window);
        TTF_Quit();
        SDL_Quit();
        return -1;
    }

    SDL_StartTextInput(window);

    TTF_Font *font = TTF_OpenFont("resources/arial.ttf", 20);
    if (!font) {
        SDL_Log("Failed to load font. Continuing without text.");
    }

    InputField input_field = create_input_field(300, 200, 200, 36, font ? font : NULL);
    Button btn = create_button(renderer, 300, 250, 200, 36, "Conectar", font ? font : NULL);

    initialize_level_elements();
    initialize_monkey();

    SDL_Event e;
    int running = 1;
    while (running) {
        // Obtener estado del teclado
        const _Bool* keyboard_state = SDL_GetKeyboardState(NULL);

        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_EVENT_QUIT) running = 0;

            // Handle input field events - FIXED FOR SDL3
            if (e.type == SDL_EVENT_MOUSE_BUTTON_DOWN) {
                // SDL3 uses different field names
                float mouse_x, mouse_y;
                SDL_GetMouseState(&mouse_x, &mouse_y);
                // SDL_Log("Mouse click at (%f, %f)", mouse_x, mouse_y);

                handle_input_field_click(&input_field, mouse_x, mouse_y);
                // SDL_Log("Input field active: %d", input_field.is_active);
            }
            else if (e.type == SDL_EVENT_TEXT_INPUT) {
                // SDL_Log("Text input: %s", e.text.text);
                if (input_field.is_active) {
                    handle_input_field_text(&input_field, e.text.text);
                }
            }
            else if (e.type == SDL_EVENT_KEY_DOWN) {
                if (e.key.key == SDLK_BACKSPACE && input_field.is_active) {
                    // SDL_Log("Backspace pressed");
                    handle_input_field_backspace(&input_field);
                }
                else if (e.key.key == SDLK_RETURN && input_field.is_active) {
                    input_field.is_active = 0;
                }
            }

            // Handle button events - UPDATE FOR SDL3
            if (button_handle_event(&btn, &e)) {
                if (!connected) {
                    SDL_Log("Intentando conectar...");
                    SDL_Log("Nombre Jugador: %s", input_field.text);

                    // Make a copy of the player name to avoid thread issues
                    char *player_name = SDL_strdup(input_field.text);
                    if (!player_name) {
                        SDL_Log("Error copying player name");
                        return 0; // or continue
                    }

                    // Start the socket thread with the player name
                    SDL_Thread *thread = SDL_CreateThread(socket_thread, "SocketThread", (void*)player_name);
                    if (!thread) {
                        SDL_Log("Error creating socket thread");
                        SDL_free(player_name);
                    }
                    // Don't send_message here - the socket thread should handle initial communication

                } else {
                    SDL_Log("Reintentando conexión...");
                    retry_connection("127.0.0.1"); // Or use a stored IP
                }
            }
        }


        SDL_SetRenderDrawColor(renderer, 30, 30, 30, 255);
        SDL_RenderClear(renderer);

        if (!connected) {
            draw_connect(renderer, &btn, &input_field);
        } else {
            draw_game(renderer, keyboard_state);
        }

        // SDL_RenderPresent(renderer);
    }

    SDL_StopTextInput(window);
    destroy_input_field(&input_field);
    destroy_button(&btn);
    if (font) TTF_CloseFont(font);
    TTF_Quit();
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}

int main() {
    return window();
}