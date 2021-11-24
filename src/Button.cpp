#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#include "Button.hpp"

Button::Button(int posx, int posy, int width, int height, const char *text, TTF_Font *font, SDL_Color color_text)
    :posx(posx), posy(posy), width(width), height(height), text(text), font(font), color_text(color_text){}

void Button::create(SDL_Renderer *renderer){
    SDL_Surface *text_surface = TTF_RenderText_Blended(font, text, color_text);
    SDL_Texture *text_texture = SDL_CreateTextureFromSurface(renderer, text_surface);

    SDL_FreeSurface(text_surface);

    SDL_Rect source_texture;
    SDL_Rect dest_texture;

    source_texture.x = source_texture.y = 0;

    SDL_QueryTexture(text_texture, NULL, NULL, &source_texture.w, &source_texture.h);

    SDL_Rect box_texture = {
        posx,
        posy,
        width,
        height
    };

    SDL_RenderDrawRect(renderer, &box_texture);

    dest_texture.x = posx + (box_texture.w - source_texture.w) / 2;
    dest_texture.y = posy + (box_texture.h - source_texture.h) / 2;

    dest_texture.w = source_texture.w;
    dest_texture.h = source_texture.h;

    SDL_RenderCopy(renderer, text_texture, &source_texture, &dest_texture);

    SDL_DestroyTexture(text_texture);
}

int Button::clicked(int mouse_x, int mouse_y){
    if(mouse_x >= posx && mouse_x <= posx + width){
        if(mouse_y >= posy && mouse_y <= posy + height){
            return 1;
        }
    }

    return 0;
}
