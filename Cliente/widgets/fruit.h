//
// Created by Joel on 22/11/2025.
//

#ifndef CLIENTETCP_FRUIT_H
#define CLIENTETCP_FRUIT_H
#include "SDL3/SDL_rect.h"
#include "SDL3/SDL_render.h"
#include "SDL3_ttf/SDL_ttf.h"

// typedef struct {
//     SDL_FRect rect;
//     SDL_Texture* texture;
//     int type; // 0 = apple, 1 = banana, 2 = peach
// } Fruit;

typedef struct {
    SDL_FRect rect;
    SDL_Texture* texture;
    int type; // 0 = apple, 1 = banana, 2 = peach
} Fruit;

Fruit create_apple(int x, int y);
Fruit create_banana(int x, int y);
Fruit create_peach(int x, int y);
Fruit create_fruit(int type, int x, int y);
void draw_fruit(Fruit* fruit);
void destroy_fruit(Fruit* fruit);

#endif //CLIENTETCP_FRUIT_H