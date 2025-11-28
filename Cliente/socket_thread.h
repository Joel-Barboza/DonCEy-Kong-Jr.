//
// Created by Joelb on 8/11/2025.
//
#pragma once

#include <stdbool.h>
#include <winsock2.h>

#include "SDL3/SDL_mutex.h"
#ifndef CLIENTETCP_SOCKET_THREAD_H
#define CLIENTETCP_SOCKET_THREAD_H

typedef struct {
    float x, y, w, h;
} Movement;

typedef enum {
    MENU,
    PLAYER,
    SPECTATOR
} GameMode;

typedef struct {
    char* game_mode;
    char* name;
} ThreadArgs;

extern GameMode current_mode;

extern Movement latest_movement;
extern int mv_updated;
extern SDL_Mutex *mv_mutex;

int start_socket(void *data);
int send_message(const char *message);
int send_movement_info(const float *x, const float *y, const float *w, const float *h);
int receive_message(char *buffer, size_t buffer_size, int blocking);
int is_connected();
int request_player_info();
bool parse_widged_placement_JSON(char* json, int* type, int *x, int *y, int *w, int *h, int *vine);
int retry_connection(const char *ip);
void close_connection();

#endif //CLIENTETCP_SOCKET_THREAD_H
