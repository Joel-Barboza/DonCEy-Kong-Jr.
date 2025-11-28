//
// Created by Joel on 20/11/2025.
//

#ifndef CLIENTETCP_TEXT_FIELD_H
#define CLIENTETCP_TEXT_FIELD_H

#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>

#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>

typedef struct {
    SDL_FRect rect;
    char text[256];
    int is_active;
    SDL_Color text_color;
    SDL_Color background_color;
    SDL_Color border_color;
    TTF_Font *font;
    SDL_Texture *textTexture;
    SDL_FRect textRect;
} InputField;

InputField create_input_field(float x, float y, float w, float h, TTF_Font *font);
void draw_input_field(InputField *field);
void handle_input_field_click(InputField *field, float mouse_x, float mouse_y);
void handle_input_field_text(InputField *field, const char *text);
void handle_input_field_backspace(InputField *field);
void destroy_input_field(InputField *field);

#endif
