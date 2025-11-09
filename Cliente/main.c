#include <stdio.h>
#include <stdlib.h>
#include <string.h>
// #define SDL_MAIN_HANDLED
#include <SDL3/SDL.h>
#include "socket_thread.h"

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

    _beginthread((void(*)(void*))socket_thread, 0, NULL);
    window();
    return 0;
}