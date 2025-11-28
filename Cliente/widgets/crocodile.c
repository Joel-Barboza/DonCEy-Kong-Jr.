//
// Created by Joel on 28/11/2025.
//

#include "crocodile.h"


#include "SDL3/SDL_render.h"

#include <SDL3_image/SDL_image.h>

#include "../constants.h"

extern SDL_Renderer* renderer;

Crocodile create_blue(int x, int y,  int vine) {
    return create_crocodile(0, x, y, vine);
}

Crocodile create_red(int x, int y, int vine) {
    return create_crocodile(1, x, y, vine);
};

Crocodile create_crocodile(int type, int x, int y, int vine) {
    Crocodile croco;
    croco.rect = (SDL_FRect){x-25, y-10, 50, 10};
    croco.type = type;
    croco.velocity = CROCODILE_SPEED;
    croco.vine = vine;
    croco.active = 1;
    croco.texture = NULL;

    const char* image_path = NULL;

    switch(type) {
        case 0: // blue
            image_path = "resources/images/blue_crocodile.png";
            break;
        case 1: // red
            image_path = "resources/images/red_crocodile.png";
            break;

        default:
            image_path = "resources/images/crocodile.png";
            break;
    }

    // Load the croco image
    SDL_Surface* surface = IMG_Load(image_path);
    if (surface) {
        croco.texture = SDL_CreateTextureFromSurface(renderer, surface);
        SDL_DestroySurface(surface);
    }

    return croco;
}

void draw_crocodile(Crocodile* croco) {
    if (croco->texture) {
        SDL_RenderTexture(renderer, croco->texture, NULL, &croco->rect);
    } else {
        // Fallback: draw a colored rectangle if texture failed to load
        SDL_Color color;
        switch(croco->type) {
            case 0: color = (SDL_Color){250, 10, 23, 255}; break; // red for apple
            case 1: color = (SDL_Color){10, 23, 255, 255}; break; // yellow for banana
            default: color = (SDL_Color){250, 10, 23, 255}; break;
        }
        SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
        SDL_RenderFillRect(renderer, &croco->rect);
    }
}

void destroy_crocodile(Crocodile* croco) {
    if (croco->texture) {
        SDL_DestroyTexture(croco->texture);
        croco->texture = NULL;
    }
}

