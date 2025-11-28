#include "button.h"
#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <string.h>

extern SDL_Renderer* renderer;

Button create_button(int x, int y, int w, int h, const char *text, TTF_Font *font) {
    Button b;
    b.rect = (SDL_FRect){x, y, w, h};
    b.color = (SDL_Color){100, 149, 237, 255};  // cornflower blue
    b.hoverColor = (SDL_Color){70, 130, 180, 255};
    b.text = text;
    b.hovered = 0;
    b.font = font;
    b.textTexture = NULL;

    SDL_Surface *textSurface = TTF_RenderText_Blended(
        font,
        text,
        SDL_strlen(text),
        (SDL_Color){255, 255, 255, 255}
    );

    if (textSurface) {
        b.textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
        b.textRect.w = textSurface->w;
        b.textRect.h = textSurface->h;
        b.textRect.x = x + (w - b.textRect.w) / 2.0f;
        b.textRect.y = y + (h - b.textRect.h) / 2.0f;
        SDL_DestroySurface(textSurface);
    }

    return b;
}

void draw_button(Button *btn) {
    SDL_SetRenderDrawColor(renderer,
        btn->hovered ? btn->hoverColor.r : btn->color.r,
        btn->hovered ? btn->hoverColor.g : btn->color.g,
        btn->hovered ? btn->hoverColor.b : btn->color.b,
        255);
    SDL_RenderFillRect(renderer, &btn->rect);

    if (btn->textTexture) {
        SDL_RenderTexture(renderer, btn->textTexture, NULL, &btn->textRect);
    }
}

int button_handle_event(Button *btn, SDL_Event *e) {
    if (e->type == SDL_EVENT_MOUSE_MOTION) {
        int mx = e->motion.x;
        int my = e->motion.y;
        btn->hovered = SDL_PointInRectFloat(&(SDL_FPoint){(float)mx, (float)my}, &btn->rect);
    } else if (e->type == SDL_EVENT_MOUSE_BUTTON_DOWN) {
        int mx = e->button.x;
        int my = e->button.y;
        if (SDL_PointInRectFloat(&(SDL_FPoint){(float)mx, (float)my}, &btn->rect)) {
            return 1; // clicked
        }
    }
    return 0;
}

void destroy_button(Button *btn) {
    if (btn->textTexture) {
        SDL_DestroyTexture(btn->textTexture);
        btn->textTexture = NULL;
    }
}
