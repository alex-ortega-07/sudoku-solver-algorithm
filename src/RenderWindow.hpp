#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <iostream>

class RenderWindow{
    public:
        RenderWindow(const char *screen_title, int sreen_width, int screen_height);
        ~RenderWindow();

        SDL_Renderer *getRenderer();

        bool loadFromRenderedText(TTF_Font *font, std::string text, SDL_Color color);

        void clear();
        void update();

    private:
        SDL_Window *window = nullptr;
        SDL_Renderer *renderer = nullptr;
};
