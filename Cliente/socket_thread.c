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


extern SDL_Renderer* renderer;
extern Fruit fruits[MAX_FRUITS];
extern int fruit_count;

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

    cJSON_AddStringToObject(json, "type", type);
    cJSON_AddStringToObject(json, "name", name);

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

    if (connect(*sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        printf("Conexion fallida a %s:%d\n", ip, port);
        return -1;
    }

    printf("Conectado al servidor %s:%d!\n", ip, port);
    connected = 1;
    return 0;
}


// returns true if parsing was successful
bool fruit_JSON(char* json, int* type, int *x, int *y, int *w, int *h) {
    cJSON *parsed_json = cJSON_Parse(json);
    if (!parsed_json) return false;

    cJSON *rect = cJSON_GetObjectItemCaseSensitive(parsed_json, "rectangle");
    if (!rect) {
        cJSON_Delete(parsed_json);
        return false;
    }

    cJSON *jtype = cJSON_GetObjectItemCaseSensitive(rect, "type");
    cJSON *jx = cJSON_GetObjectItemCaseSensitive(rect, "x");
    cJSON *jy = cJSON_GetObjectItemCaseSensitive(rect, "y");
    cJSON *jw = cJSON_GetObjectItemCaseSensitive(rect, "width");
    cJSON *jh = cJSON_GetObjectItemCaseSensitive(rect, "height");

    if (!cJSON_IsNumber(jx) || !cJSON_IsNumber(jy) ||
        !cJSON_IsNumber(jw) || !cJSON_IsNumber(jh)) {

        cJSON_Delete(parsed_json);
        return false;
        }

    *type = jtype->valueint;
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




int socket_thread(void *data) {
    char *player_name = (char *)data;
    const char *server_ip = "127.0.0.1"; // Default IP, or get from another input field

#ifdef _WIN32
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        printf("Error inicializando Winsock\n");
        SDL_free(player_name);
        return -1;
    }
#endif

    if (global_socket != INVALID_SOCKET) {
        closesocket(global_socket);
    }

    global_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (global_socket == INVALID_SOCKET) {
        printf("Error creando socket\n");
        SDL_free(player_name);
#ifdef _WIN32
        WSACleanup();
#endif
        return -1;
    }

    if (connect_to_server(&global_socket, server_ip, PORT) < 0) {
        closesocket(global_socket);
        global_socket = INVALID_SOCKET;
        SDL_free(player_name);
#ifdef _WIN32
        WSACleanup();
#endif
        return -1;
    }

    // Send player name as JSON
    if (player_name && strlen(player_name) > 0) {
        send_subscriber_info("PLAYER", player_name);
    }

    char buffer[BUFFER_SIZE] = {0};

    // Receive welcome message or game state
    int valread = receive_message(buffer, BUFFER_SIZE, 1);
    if (valread > 0) {
        printf("Servidor: %s\n", buffer);
    }

    // Main communication loop for the game
    while (connected) {
        // Receive game updates from server
        memset(buffer, 0, BUFFER_SIZE);
        valread = receive_message(buffer, BUFFER_SIZE, 0); // Non-blocking
        if (valread > 0) {
            printf("Servidor: %s\n", buffer);

            int type, x, y, w, h;

            // const char* json = "{\"rectangle\":{\"x\":80,\"width\":6,\"y\":78,\"height\":300}}";

            if (fruit_JSON(buffer, &type, &x, &y, &w, &h)) {
                printf("Rectangle: x=%d y=%d w=%d h=%d\n", x, y, w, h);

                // Fruit fruit = create_fruit(type, x, y);
                // draw_fruit(&fruit);
                add_fruit(type, x, y);
                // SDL_RenderPresent(renderer);
            } else {
                printf("Invalid JSON\n");
            }

            // Handle game state updates here
        } else if (valread < 0) {
            break; // Connection error
        }

        // Small delay to prevent busy waiting
        SDL_Delay(10);
    }

    if (global_socket != INVALID_SOCKET) {
        closesocket(global_socket);
        global_socket = INVALID_SOCKET;
    }

    // Free the player name memory
    if (player_name) {
        SDL_free(player_name);
    }

#ifdef _WIN32
    WSACleanup();
#endif
    connected = 0;
    return 0;
}



// Updated retry_connection to use a specific IP
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