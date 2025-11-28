//
// Created by Joel on 22/11/2025.
//

#include "fruit.h"

#include "SDL3/SDL_render.h"

#include <SDL3_image/SDL_image.h>

extern SDL_Renderer* renderer;

Fruit create_apple(int x, int y) {
    return create_fruit(0, x, y);
}

Fruit create_banana(int x, int y) {
    return create_fruit(1, x, y);
}

Fruit create_peach(int x, int y) {
    return create_fruit(2, x, y);
}

Fruit create_fruit(int type, int x, int y) {
    Fruit fruit;
    fruit.rect = (SDL_FRect){x-20, y-20, 40, 40};
    fruit.type = type;
    fruit.texture = NULL;

    const char* image_path = NULL;

    switch(type) {
        case 0: // apple
            image_path = "resources/images/apple.png";
            break;
        case 1: // peach
            image_path = "resources/images/peach.png";
            break;
        case 2: // banana
            image_path = "resources/images/banana.png";
            break;
        default:
            image_path = "resources/images/apple.png";
            break;
    }

    // Load the fruit image
    SDL_Surface* surface = IMG_Load(image_path);
    if (surface) {
        fruit.texture = SDL_CreateTextureFromSurface(renderer, surface);
        SDL_DestroySurface(surface);
    }

    return fruit;
}

void draw_fruit(Fruit* fruit) {
    if (fruit->texture) {
        SDL_RenderTexture(renderer, fruit->texture, NULL, &fruit->rect);
    } else {
        // Fallback: draw a colored rectangle if texture failed to load
        SDL_Color color;
        switch(fruit->type) {
            case 0: color = (SDL_Color){250, 10, 23, 255}; break; // red for apple
            case 1: color = (SDL_Color){255, 255, 0, 255}; break; // yellow for banana
            case 2: color = (SDL_Color){255, 218, 185, 255}; break; // peach color
            default: color = (SDL_Color){250, 10, 23, 255}; break;
        }
        SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
        SDL_RenderFillRect(renderer, &fruit->rect);
    }
}

void destroy_fruit(Fruit* fruit) {
    if (fruit->texture) {
        SDL_DestroyTexture(fruit->texture);
        fruit->texture = NULL;
    }
}

