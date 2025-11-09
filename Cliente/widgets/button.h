//
// Created by Joelb on 8/11/2025.
//

#ifndef CLIENTETCP_BUTTON_H
#define CLIENTETCP_BUTTON_H


#include <SDL3/SDL.h>

typedef struct {
    SDL_Rect rect;             // position and size
    SDL_Color color;           // normal color
    SDL_Color hoverColor;      // when mouse is over
    const char *text;          // optional label
    int hovered;               // internal flag
} Button;

// Initialize a button
Button create_button(int x, int y, int w, int h, const char *text);

// Draw button
void draw_button(SDL_Renderer *renderer, Button *btn);

// Check if button is clicked
int button_handle_event(Button *btn, SDL_Event *e);

#endif //CLIENTETCP_BUTTON_H