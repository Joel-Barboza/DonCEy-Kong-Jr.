//
// Created by Joelb on 9/11/2025.
//

#include "button.h"
#include <SDL3/SDL.h>

Button create_button(int x, int y, int w, int h, const char *text) {
    Button b;
    b.rect = (SDL_Rect){x, y, w, h};
    b.color = (SDL_Color){100, 149, 237, 255};  // cornflower blue
    b.hoverColor = (SDL_Color){70, 130, 180, 255};
    b.text = text;
    b.hovered = 0;
    return b;
}

void draw_button(SDL_Renderer *renderer, Button *btn) {
    SDL_SetRenderDrawColor(renderer,
        btn->hovered ? btn->hoverColor.r : btn->color.r,
        btn->hovered ? btn->hoverColor.g : btn->color.g,
        btn->hovered ? btn->hoverColor.b : btn->color.b,
        255);
    SDL_RenderFillRect(renderer, &btn->rect);
}

int button_handle_event(Button *btn, SDL_Event *e) {
    if (e->type == SDL_EVENT_MOUSE_MOTION) {
        int mx = e->motion.x;
        int my = e->motion.y;
        btn->hovered = SDL_PointInRect(&(SDL_Point){mx, my}, &btn->rect);
    } else if (e->type == SDL_EVENT_MOUSE_BUTTON_DOWN) {
        int mx = e->button.x;
        int my = e->button.y;
        if (SDL_PointInRect(&(SDL_Point){mx, my}, &btn->rect)) {
            return 1; // clicked
        }
    }
    return 0;
}
