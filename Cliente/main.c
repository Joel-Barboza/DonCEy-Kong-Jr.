#include <stdio.h>
#include <stdlib.h>
#include <string.h>
// #define SDL_MAIN_HANDLED
#include <SDL3/SDL.h>

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


int window()
{
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        SDL_Log("SDL init failed: %s", SDL_GetError());
        return -1;
    }

    SDL_Window *window = SDL_CreateWindow("Hello", 800, 600, 0);
    SDL_Renderer *renderer = SDL_CreateRenderer(window, NULL);

    SDL_Event e;
    int running = 1;
    while (running) {
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_EVENT_QUIT) running = 0;
        }

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);
        SDL_RenderPresent(renderer);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}


int main() {

    window();

    #ifdef _WIN32
        WSADATA wsaData;
        if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
            printf("Error inicializando Winsock\n");
            return -1;
        }
    #endif

    SOCKET sock = 0;
    struct sockaddr_in serv_addr;
    char buffer[BUFFER_SIZE] = {0};
    char message[BUFFER_SIZE];

    // Crear socket
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET) {
        printf("\nError creando socket\n");
        #ifdef _WIN32
            WSACleanup();
        #endif
        return -1;
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);

    // Convertir dirección IP
    if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0) {
        printf("\nDireccion invalida\n");
        close(sock);
        #ifdef _WIN32
            WSACleanup();
        #endif
        return -1;
    }

    // Conectar al servidor
    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        printf("\nConexion fallida\n");
        close(sock);
        #ifdef _WIN32
            WSACleanup();
        #endif
        return -1;
    }

    printf("Conectado al servidor!\n");

    // Recibir mensaje de bienvenida
    int valread = recv(sock, buffer, BUFFER_SIZE - 1, 0);
    if (valread > 0) {
        buffer[valread] = '\0';  // Asegurar terminación nula
        printf("Servidor: %s\n", buffer);
    }

    while (1) {
        printf("Tu mensaje: ");
        fgets(message, BUFFER_SIZE, stdin);

        // Eliminar salto de línea
        message[strcspn(message, "\n")] = 0;

        // Enviar mensaje al servidor (agregar \n para que Java lo lea)
        char message_with_newline[BUFFER_SIZE + 2];
        snprintf(message_with_newline, sizeof(message_with_newline), "%s\n", message);
        send(sock, message_with_newline, strlen(message_with_newline), 0);

        if (strcmp(message, "quit") == 0) {
            printf("Saliendo...\n");
            break;
        }

        // Limpiar buffer
        memset(buffer, 0, BUFFER_SIZE);

        // Recibir respuesta del servidor
        valread = recv(sock, buffer, BUFFER_SIZE - 1, 0);
        if (valread > 0) {
            buffer[valread] = '\0';  // Asegurar terminación nula

            // Eliminar \n y \r si existen
            char *pos;
            if ((pos = strchr(buffer, '\n')) != NULL) *pos = '\0';
            if ((pos = strchr(buffer, '\r')) != NULL) *pos = '\0';

            printf("Servidor: %s\n", buffer);
        } else if (valread == 0) {
            printf("Servidor cerró la conexión\n");
            break;
        } else {
            printf("Error recibiendo datos\n");
            break;
        }
    }

    close(sock);

    #ifdef _WIN32
        WSACleanup();
    #endif

    return 0;
}