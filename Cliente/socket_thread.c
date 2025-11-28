#include "socket_thread.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <io.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <SDL3/SDL.h>

#include "cjson/cJSON.h"
#include "widgets/fruit.h"
#include "constants.h"
#include "widgets/button.h"
#include "widgets/crocodile.h"


extern SDL_Renderer *renderer;
extern Fruit fruits[MAX_FRUITS];
extern int fruit_count;
extern Crocodile crocodiles[MAX_CROCODILES];
extern int crocodile_count;
Movement latest_movement = {0, 0, 0, 0};
int mv_updated = 0;
SDL_Mutex *mv_mutex = NULL;
GameMode current_mode = MENU;



static SOCKET global_socket = INVALID_SOCKET;
int connected = 0;

// Function to send a message
int send_message(const char *message) {
    if (!connected || global_socket == INVALID_SOCKET) {
        printf("No hay conexion activa\n");
        return -1;
    }

    // Add newline if not present
    char message_with_newline[BUFFER_SIZE + 2];
    size_t message_len = strlen(message);

    if (message_len >= BUFFER_SIZE) {
        printf("Mensaje demasiado largo\n");
        return -1;
    }

    // Check if message already ends with newline
    if (message_len > 0 && message[message_len - 1] == '\n') {
        strncpy(message_with_newline, message, sizeof(message_with_newline) - 1);
        message_with_newline[sizeof(message_with_newline) - 1] = '\0';
    } else {
        snprintf(message_with_newline, sizeof(message_with_newline), "%s\n", message);
    }

    int result = send(global_socket, message_with_newline, strlen(message_with_newline), 0);
    if (result == SOCKET_ERROR) {
        printf("Error enviando mensaje: %d\n", WSAGetLastError());
        connected = 0;
        closesocket(global_socket);
        global_socket = INVALID_SOCKET;
        return -1;
    }

    printf("Mensaje enviado: %s", message_with_newline);
    return result;
}

// Function to receive a message (non-blocking option)
int receive_message(char *buffer, size_t buffer_size, int blocking) {
    if (!connected || global_socket == INVALID_SOCKET) {
        return -1;
    }

    // Set socket to non-blocking if requested
    u_long mode = blocking ? 0 : 1;
    ioctlsocket(global_socket, FIONBIO, &mode);

    int valread = recv(global_socket, buffer, buffer_size - 1, 0);

    if (valread > 0) {
        buffer[valread] = '\0';
        // Remove trailing newline if present
        if (valread > 0 && buffer[valread - 1] == '\n') {
            buffer[valread - 1] = '\0';
        }
        return valread;
    } else if (valread == 0) {
        printf("Servidor cerro la conexion\n");
        connected = 0;
        closesocket(global_socket);
        global_socket = INVALID_SOCKET;
        return 0;
    } else {
        int error = WSAGetLastError();
        if (error == WSAEWOULDBLOCK && !blocking) {
            // No data available in non-blocking mode
            return 0;
        }
        printf("Error recibiendo datos: %d\n", error);
        connected = 0;
        closesocket(global_socket);
        global_socket = INVALID_SOCKET;
        return -1;
    }
}

// Check connection status
int is_connected() {
    if (!connected || global_socket == INVALID_SOCKET) {
        return 0;
    }

    // Simple check using non-blocking receive
    char temp_buffer[1];
    int result = recv(global_socket, temp_buffer, 1, MSG_PEEK);
    if (result == 0) {
        connected = 0;
        return 0;
    } else if (result == SOCKET_ERROR) {
        int error = WSAGetLastError();
        if (error != WSAEWOULDBLOCK) {
            connected = 0;
            return 0;
        }
    }

    return 1;
}

int send_subscriber_info(const char *type, const char *name) {
    cJSON *json = cJSON_CreateObject();
    if (!json) return -1;

    cJSON_AddStringToObject(json, "msg_type", "subs_type");
    cJSON_AddStringToObject(json, "type", type);
    if (strcmp(type, "PLAYER") == 0) {
        cJSON_AddStringToObject(json, "name", name);
    }


    char *json_str = cJSON_PrintUnformatted(json);
    if (!json_str) {
        cJSON_Delete(json);
        return -1;
    }

    int result = send_message(json_str);
    free(json_str);
    cJSON_Delete(json);
    return result;
}

int request_player_info() {
    cJSON *json = cJSON_CreateObject();
    if (!json) return -1;

    cJSON_AddStringToObject(json, "msg_type", "request_players");

    char *json_str = cJSON_PrintUnformatted(json);
    if (!json_str) {
        cJSON_Delete(json);
        return -1;
    }

    int result = send_message(json_str);
    free(json_str);
    cJSON_Delete(json);
    return result;
}

int send_movement_info(const float *x, const float *y, const float *w, const float *h) {
    cJSON *json = cJSON_CreateObject();
    if (!json) return -1;

    cJSON_AddStringToObject(json, "msg_type", "movement");

    cJSON *rect = cJSON_CreateObject();
    if (!rect) {
        cJSON_Delete(json);
        return -1;
    }

    cJSON_AddNumberToObject(rect, "x", *x);
    cJSON_AddNumberToObject(rect, "y", *y);
    cJSON_AddNumberToObject(rect, "width", *w);
    cJSON_AddNumberToObject(rect, "height", *h);

    cJSON_AddItemToObject(json, "dkj_rect", rect);

    char *json_str = cJSON_PrintUnformatted(json);
    if (!json_str) {
        cJSON_Delete(json);
        return -1;
    }

    int result = send_message(json_str);

    free(json_str);
    cJSON_Delete(json);

    return result;
}


// Updated connect_to_server function that uses the IP parameter
int connect_to_server(SOCKET *sock, const char *ip, int port) {
    struct sockaddr_in serv_addr;
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(port);

    if (inet_pton(AF_INET, ip, &serv_addr.sin_addr) <= 0) {
        printf("Direccion invalida: %s\n", ip);
        return -1;
    }

    if (connect(*sock, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) {
        printf("Conexion fallida a %s:%d\n", ip, port);
        return -1;
    }

    printf("Conectado al servidor %s:%d!\n", ip, port);
    connected = 1;

    return 0;
}


// returns true if parsing was successful
bool parse_widged_placement_JSON(char *json,int *type, int *x, int *y, int *w, int *h, int *vine) {
    cJSON *parsed_json = cJSON_Parse(json);
    if (!parsed_json) return false;

    cJSON *rect = cJSON_GetObjectItemCaseSensitive(parsed_json, "rectangle");
    if (!rect) {
        cJSON_Delete(parsed_json);
        return false;
    }


    // cJSON *j_msg_type = cJSON_AddStringToObject(parsed_json, "msg_type", wid_type);
    //

    cJSON *jtype = cJSON_GetObjectItemCaseSensitive(parsed_json, "type");
    cJSON *jvine = cJSON_GetObjectItemCaseSensitive(parsed_json, "vine");
    cJSON *jx = cJSON_GetObjectItemCaseSensitive(rect, "x");
    cJSON *jy = cJSON_GetObjectItemCaseSensitive(rect, "y");
    cJSON *jw = cJSON_GetObjectItemCaseSensitive(rect, "width");
    cJSON *jh = cJSON_GetObjectItemCaseSensitive(rect, "height");

    if (!cJSON_IsNumber(jx) || !cJSON_IsNumber(jy) ||
        !cJSON_IsNumber(jw) || !cJSON_IsNumber(jh) || !cJSON_IsNumber(jvine)) {
        cJSON_Delete(parsed_json);
        return false;
    }

    *type = jtype->valueint;
    *vine = jvine->valueint;
    *x = jx->valueint;
    *y = jy->valueint;
    *w = jw->valueint;
    *h = jh->valueint;

    cJSON_Delete(parsed_json);
    return true;
}



void add_fruit(int type, int x, int y) {
    if (fruit_count >= MAX_FRUITS) return;

    fruits[fruit_count] = create_fruit(type, x, y);
    fruit_count++;
}

void add_crocodile(int type, int x, int y, int vine) {
    if (crocodile_count >= MAX_CROCODILES) return;

    crocodiles[crocodile_count] = create_crocodile(type, x, y, vine);
    crocodile_count++;
}

// {"players":[{"name":"asdf"}],"msg_type":"player_list"}


int start_socket(void *data) {
    ThreadArgs *args = (ThreadArgs *) data;

    char *game_mode = args->game_mode;
    char *name = args->name;
    const char *server_ip = "127.0.0.1"; // Default IP

#ifdef _WIN32
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        printf("Error inicializando Winsock\n");
        SDL_free(game_mode);
        SDL_free(name);
        return -1;
    }
#endif

    if (global_socket != INVALID_SOCKET) {
        closesocket(global_socket);
    }

    global_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (global_socket == INVALID_SOCKET) {
        printf("Error creando socket\n");
        SDL_free(game_mode);
        SDL_free(name);
#ifdef _WIN32
        WSACleanup();
#endif
        return -1;
    }

    if (connect_to_server(&global_socket, server_ip, PORT) < 0) {
        closesocket(global_socket);
        global_socket = INVALID_SOCKET;
        SDL_free(game_mode);
        SDL_free(name);
#ifdef _WIN32
        WSACleanup();
#endif
        return -1;
    }

    if (current_mode == SPECTATOR) {
        request_player_info();
    }
    // Send player name as JSON
    if (game_mode && strlen(game_mode) > 0) {
        send_subscriber_info(game_mode, name);
    }

    char buffer[BUFFER_SIZE] = {0};

    // Receive welcome message or game state
    int valread = receive_message(buffer, BUFFER_SIZE, 1);
    if (valread > 0) {
        printf("Servidor: %s\n", buffer);
    }

    uint32_t last_send_time = SDL_GetTicks();

    // Main communication loop for the game
    while (connected) {
        memset(buffer, 0, BUFFER_SIZE);
        valread = receive_message(buffer, BUFFER_SIZE, 0);

        if (valread < 0)
            break;

        if (valread > 0) {
            cJSON *parsed_json = cJSON_Parse(buffer);
            if (!parsed_json) {
                SDL_Log("Invalid JSON");
                continue;
            }

            cJSON *msg_type = cJSON_GetObjectItemCaseSensitive(parsed_json, "msg_type");
            if (!msg_type || !cJSON_IsString(msg_type)) {
                cJSON_Delete(parsed_json);
                continue;
            }

            int type, x, y, w, h, vine;


            // ===== FRUITS =====
            if (strcmp(msg_type->valuestring, "place_fruit") == 0 && parse_widged_placement_JSON(buffer, &type, &x, &y, &w, &h, &vine)) {
                // printf("Rectangle: x=%d y=%d w=%d h=%d\n", x, y, w, h);

                add_fruit(type, x, y);
            } else {
                printf("Invalid JSON\n");
            }

            // ===== Crocodiles =====
            if (strcmp(msg_type->valuestring, "place_crocodile") == 0 && parse_widged_placement_JSON(buffer, &type, &x, &y, &w, &h, &vine)) {
                // printf("Rectangle: x=%d y=%d w=%d h=%d\n", x, y, w, h);

                add_crocodile(type, x, y, vine);
            } else {
                printf("Invalid JSON\n");
            }

            // ===== PLAYER LIST =====
            if (strcmp(msg_type->valuestring, "player_list") == 0) {
                cJSON *players = cJSON_GetObjectItemCaseSensitive(parsed_json, "players");
                if (players && cJSON_IsArray(players)) {
                    printf("Players online: %d\n", cJSON_GetArraySize(players));

                    TTF_Font *font = TTF_OpenFont("resources/arial.ttf", 20);
                    if (!font) {
                        SDL_Log("Failed to load font. Continuing without text.");
                    }
                    for (int i = 0; i < cJSON_GetArraySize(players); i++) {
                    }

                    for (int i = 0; i < cJSON_GetArraySize(players); i++) {
                        cJSON *player = cJSON_GetArrayItem(players, i);
                        cJSON *name = cJSON_GetObjectItemCaseSensitive(player, "name");
                        if (name && cJSON_IsString(name)) {
                            Button btn = create_button(50 * (i + 1), 50 * (i + 1), 200, 36, name->valuestring, font ? font : NULL);
                            draw_button(&btn);
                            // printf(" - %s\n", name->valuestring);
                        }
                    }
                }
            }


            cJSON_Delete(parsed_json);
        }

        // SEND MOVEMENT (every 50 ms)
        uint32_t now = SDL_GetTicks();
        if (current_mode == PLAYER && now - last_send_time >= 50) {
            last_send_time = now;

            SDL_LockMutex(mv_mutex);
            if (mv_updated) {
                send_movement_info(&latest_movement.x,
                                   &latest_movement.y,
                                   &latest_movement.w,
                                   &latest_movement.h);

                mv_updated = 0;
            }
            SDL_UnlockMutex(mv_mutex);
        }

        SDL_Delay(30);
    }


    if (global_socket != INVALID_SOCKET) {
        closesocket(global_socket);
        global_socket = INVALID_SOCKET;
    }

    SDL_free(args->game_mode);
    SDL_free(args->name);
    free(args);

#ifdef _WIN32
    WSACleanup();
#endif
    connected = 0;
    return 0;
}


// retry_connection with a specific IP
int retry_connection(const char *ip) {
    if (ip == NULL || strlen(ip) == 0) {
        ip = "127.0.0.1";
    }

    if (global_socket == INVALID_SOCKET) {
        global_socket = socket(AF_INET, SOCK_STREAM, 0);
        if (global_socket == INVALID_SOCKET) {
            printf("Error creando socket\n");
            return -1;
        }
    }

    if (connect_to_server(&global_socket, ip, PORT) == 0) {
        printf("Reconectado al servidor %s!\n", ip);
        return 0;
    } else {
        printf("Reconexion fallida a %s\n", ip);
        return -1;
    }
}

// Function to close the connection cleanly
void close_connection() {
    if (global_socket != INVALID_SOCKET) {
        // Send quit message to server
        send_message("quit");
        closesocket(global_socket);
        global_socket = INVALID_SOCKET;
    }
    connected = 0;
}
