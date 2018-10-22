#include <SDL2/SDL.h>
#include <stdio.h>

char running = 1;

int main(void) { 
    SDL_Init(SDL_INIT_VIDEO);
    SDL_Window* window;
    SDL_Renderer* renderer;
    SDL_Event e;
    window = SDL_CreateWindow("Kill me", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 640, 480, SDL_WINDOW_SHOWN);
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    fprintf(stderr, "Enter loop\n");    
    while(running) {
        //Draw-------------------------------------
        //Clear------------------------------------
        SDL_SetRenderDrawColor(renderer,247,247,247,255);
        SDL_RenderClear(renderer);
        SDL_RenderPresent(renderer);
        //Events-----------------------------------
        SDL_PollEvent(&e);
        switch(e.type) {
            case SDL_QUIT :
                running = 0;
                break;
        }
        //Inputs-----------------------------------
    }    
    fprintf(stderr, "Exit loop\n");
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}