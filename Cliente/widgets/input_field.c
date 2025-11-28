//
// Created by Joel on 20/11/2025.
//

#include "input_field.h"
#include <string.h>

extern SDL_Renderer* renderer;

InputField create_input_field(float x, float y, float w, float h, TTF_Font *font) {
    InputField field;
    field.rect = (SDL_FRect){x, y, w, h};
    field.text[0] = '\0';
    field.is_active = 0;
    field.text_color = (SDL_Color){255, 255, 255, 255};
    field.background_color = (SDL_Color){50, 50, 50, 255};
    field.border_color = (SDL_Color){100, 100, 100, 255};
    field.font = font;
    field.textTexture = NULL;
    field.textRect = (SDL_FRect){0, 0, 0, 0};

    return field;
}

void draw_input_field(InputField *field) {
    // Draw background
    SDL_SetRenderDrawColor(renderer,
        field->background_color.r,
        field->background_color.g,
        field->background_color.b,
        field->background_color.a);
    SDL_RenderFillRect(renderer, &field->rect);

    // Draw border
    SDL_SetRenderDrawColor(renderer,
        field->is_active ? 0 : field->border_color.r,
        field->is_active ? 150 : field->border_color.g,
        field->is_active ? 255 : field->border_color.b,
        255);
    SDL_RenderRect(renderer, &field->rect);

    // Always recreate the texture each frame for simplicity
    if (field->textTexture) {
        SDL_DestroyTexture(field->textTexture);
        field->textTexture = NULL;
    }

    // Draw text if exists
    if (field->text[0] != '\0' && field->font) {
        SDL_Surface *textSurface = TTF_RenderText_Blended(
            field->font,
            field->text,
            SDL_strlen(field->text),
            field->text_color
        );

        if (textSurface) {
            field->textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
            if (field->textTexture) {
                SDL_FRect textRect = {
                    field->rect.x + 5.0f,
                    field->rect.y + (field->rect.h - (float)textSurface->h) / 2.0f,
                    (float)textSurface->w,
                    (float)textSurface->h
                };
                SDL_RenderTexture(renderer, field->textTexture, NULL, &textRect);
            }
            SDL_DestroySurface(textSurface);
        }
    }

    // Draw simple cursor
    if (field->is_active) {
        static Uint32 last_blink = 0;
        static int show_cursor = 1;
        Uint32 current_time = SDL_GetTicks();

        if (current_time - last_blink > 500) {
            show_cursor = !show_cursor;
            last_blink = current_time;
        }

        if (show_cursor) {
            float cursor_x = field->rect.x + 5.0f;
            if (field->textTexture) {
                // Use the texture width to position cursor at end of text
                int w, h;
                SDL_GetTextureSize(field->textTexture, &w, &h);
                cursor_x += (float)w;
            }

            SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
            SDL_RenderLine(renderer, cursor_x, field->rect.y + 5.0f, cursor_x, field->rect.y + field->rect.h - 5.0f);
        }
    }
}

// The other functions remain the same as above
void handle_input_field_click(InputField *field, float mouse_x, float mouse_y) {
    SDL_FPoint point = {mouse_x, mouse_y};
    field->is_active = SDL_PointInRectFloat(&point, &field->rect);
}

void handle_input_field_text(InputField *field, const char *text) {
    if (field->is_active) {
        size_t current_len = SDL_strlen(field->text);
        size_t text_len = SDL_strlen(text);

        // SDL_Log("Before text: '%s', adding: '%s'", field->text, text);

        if (current_len + text_len < sizeof(field->text) - 1) {
            SDL_strlcat(field->text, text, sizeof(field->text));
            // SDL_Log("After text: '%s'", field->text);

            // Destroy old texture to force recreation
            if (field->textTexture) {
                SDL_DestroyTexture(field->textTexture);
                field->textTexture = NULL;
            }
        } else {
            SDL_Log("Text buffer full!");
        }
    }
}

void handle_input_field_backspace(InputField *field) {
    if (field->is_active) {
        size_t len = SDL_strlen(field->text);
        if (len > 0) {
            field->text[len - 1] = '\0';

            if (field->textTexture) {
                SDL_DestroyTexture(field->textTexture);
                field->textTexture = NULL;
            }
        }
    }
}

void destroy_input_field(InputField *field) {
    if (field->textTexture) {
        SDL_DestroyTexture(field->textTexture);
        field->textTexture = NULL;
    }
}