#include "SDL2/SDL.h"
#include "SDL2/SDL_image.h"
#include "SDL2/SDL_ttf.h"
#include <iostream>
#include <string>
#include <sstream>

int main(int argc, char *argv[])
{
    SDL_Window *window = NULL;
    SDL_Renderer *renderer = NULL;
    SDL_Surface *surface = NULL;
    SDL_Texture *texture = NULL;
    SDL_Event event;

    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't initialize SDL: %s", SDL_GetError());
        return 3;
    }

    // load support for the JPG and PNG image formats
    int flags = IMG_INIT_JPG|IMG_INIT_PNG;
    if( (IMG_Init(flags)&flags) != flags ) {
        std::cout << "IMG_Init: Failed to init required jpg and png support!\n";
        std::cout << "IMG_Init: " << IMG_GetError() << std::endl;
        // handle error
    }

    if(TTF_Init()==-1) {
        std::cout << "TTF_Init: " << TTF_GetError() << std::endl;
        return 3;
    }

    TTF_Font *font;
    font = TTF_OpenFont("fonts/GenJyuuGothic-Regular.ttf", 300);
    if(!font) {
        std::cout << "TTF_OpenFont: " << TTF_GetError() << std::endl;
        return 3;
        // handle error
    }
    TTF_SetFontStyle(font, TTF_STYLE_BOLD|TTF_STYLE_ITALIC);
    SDL_Color color={0,255,235};

    window = SDL_CreateWindow("Hello World", 50, 50, 1280, 120, SDL_WINDOW_SHOWN|SDL_WINDOW_RESIZABLE);
    if (!window) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't create window: %s", SDL_GetError());
        return 3;
    }

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't create renderer: %s", SDL_GetError());
        return 3;
    }

    Uint32 startTime = 0;
    std::stringstream timeText;

    while (1) {
        while( SDL_PollEvent(&event) ){
            if (event.type == SDL_QUIT) {
                goto close;
            }
            else if( event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_RETURN )
            {
                startTime = SDL_GetTicks();
            }
        }
        timeText.str( "" );
        timeText << "Milliseconds since start time " << SDL_GetTicks() - startTime;
        surface = TTF_RenderText_Solid(font, timeText.str().c_str(), color);
        if (!surface) {
            SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't create surface from image: %s", SDL_GetError());
            return 3;
        }
        texture = SDL_CreateTextureFromSurface(renderer, surface);
        if (!texture) {
            SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't create texture from surface: %s", SDL_GetError());
            return 3;
        }
        SDL_FreeSurface(surface);
        SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0x00);
        SDL_RenderClear(renderer);
        SDL_RenderCopy(renderer, texture, NULL, NULL);
        SDL_RenderPresent(renderer);
    }

    close:
    TTF_CloseFont(font);
    font = NULL;

    SDL_DestroyTexture(texture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);

    TTF_Quit();
    IMG_Quit();
    SDL_Quit();

    return 0;
}

