//
// Created by Joel on 28/11/2025.
//

#ifndef CLIENTETCP_CROCODILE_H
#define CLIENTETCP_CROCODILE_H
#include "SDL3/SDL_render.h"

typedef struct {
    SDL_FRect rect;
    SDL_Texture* texture;
    int type; // 0 = blue, 1 = red
    float velocity;
    int vine;
    int active;
} Crocodile;

Crocodile create_blue(int x, int y, int vine);
Crocodile create_red(int x, int y, int vine);
Crocodile create_crocodile(int type, int x, int y, int vine);
void draw_crocodile(Crocodile* croco);
void destroy_crocodile(Crocodile* croco);


#endif //CLIENTETCP_CROCODILE_H