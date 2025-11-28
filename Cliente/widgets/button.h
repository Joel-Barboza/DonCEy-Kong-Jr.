//
// Created by Joelb on 8/11/2025.
//

#ifndef CLIENTETCP_BUTTON_H
#define CLIENTETCP_BUTTON_H


#include <SDL3/SDL.h>
#include "SDL3_ttf/SDL_ttf.h"

typedef struct {
    SDL_FRect rect;
    SDL_Color color;
    SDL_Color hoverColor;
    const char *text;
    int hovered;
    TTF_Font *font;          // font for text
    SDL_Texture *textTexture;
    SDL_FRect textRect;
} Button;

Button create_button(int x, int y, int w, int h, const char *text, TTF_Font *font);
void draw_button(Button *btn);
int button_handle_event(Button *btn, SDL_Event *e);
void destroy_button(Button *btn);

#endif //CLIENTETCP_BUTTON_H