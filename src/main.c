#include <cstdio>
#include <stdio.h>
#define SDL_MAIN_HANDLED
#include <SDL2/SDL.h>

#include "constants.h"
#include "map.h"
#include "player.h"

SDL_Window *window = NULL;
SDL_Renderer *renderer = NULL;

bool isGameRunnig = false;
int ticksLastFrame = 0;

struct Player player;

int InitializeWindow() {
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
        fprintf(stderr, "Error initializing SDL\n");
        return false;
    }

    window =
        SDL_CreateWindow(NULL, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                         WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_BORDERLESS);

    if (!window) {
        fprintf(stderr, "Error creating SDL window\n");
        return false;
    }

    renderer = SDL_CreateRenderer(window, -1, 0);

    if (!renderer) {
        fprintf(stderr, "Error creating renderer\n");
        return false;
    }

    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);

    return true;
}

void DestroyWindow() {
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

void Setup() { player = CreatePlayer(); }

void ProcessInput() {
    SDL_Event event;
    SDL_PollEvent(&event);
    switch (event.type) {
    case SDL_QUIT: {
        isGameRunnig = false;
        break;
    }
    case SDL_KEYDOWN: {
        if (event.key.keysym.sym == SDLK_ESCAPE) {
            isGameRunnig = false;
            break;
        }
    }
    }
}

void Update() {
    int timeTillNextFrame =
        FRAME_TIME_LENGHT - (SDL_GetTicks() - ticksLastFrame);
    if (timeTillNextFrame > 0 && timeTillNextFrame <= FRAME_TIME_LENGHT) {
        SDL_Delay(timeTillNextFrame);
    }
    float deltaTime = (SDL_GetTicks() - ticksLastFrame) / 1000.0f;
    ticksLastFrame = SDL_GetTicks();
}

void Render() {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    RenderMap(renderer);

    SDL_RenderPresent(renderer);
}

int main(int argc, char **argv) {
    isGameRunnig = InitializeWindow();

    Setup();

    while (isGameRunnig) {
        ProcessInput();
        Update();
        Render();
    }

    DestroyWindow();
    return 0;
}
