#include "constants.h"
#include "engine.h"
#include "map.h"
#include "sprite.h"
#include "texture.h"
#include "wall.h"
#include <stdio.h>
#include <stdlib.h>

struct EngineData *Engine_Create(void) {
    struct EngineData *data =
        (struct EngineData *)malloc(sizeof(struct EngineData));
    data->window = NULL;
    data->renderer = NULL;
    data->isGameRunning = false;
    data->ticksLastFrame = 0;
    data->colorBuffer = NULL;
    return data;
}

void Engine_Destroy(struct EngineData *engine) {
    if (!engine)
        return;

    Texture_FreeTextures();

    if (!engine->colorBuffer)
        ColorBuffer_Destroy(engine->colorBuffer);

    if (engine->renderer)
        SDL_DestroyRenderer(engine->renderer);
    if (engine->window)
        SDL_DestroyWindow(engine->window);

    SDL_Quit();

    free(engine);
}

bool Engine_InitializeWindow(struct EngineData *engine) {

    if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
        fprintf(stderr, "Error initializing SDL\n");
        return false;
    }

    SDL_DisplayMode display_mode;
    SDL_GetCurrentDisplayMode(0, &display_mode);
    int fullScreenWidth = display_mode.w;
    int fullScreenHeight = display_mode.h;

    engine->window = SDL_CreateWindow(NULL, SDL_WINDOWPOS_CENTERED,
                                      SDL_WINDOWPOS_CENTERED, fullScreenWidth,
                                      fullScreenHeight, SDL_WINDOW_BORDERLESS);

    if (!engine->window) {
        fprintf(stderr, "Error creating SDL window\n");
        return false;
    }

    engine->renderer = SDL_CreateRenderer(engine->window, -1, 0);

    if (!engine->renderer) {
        fprintf(stderr, "Error creating renderer\n");
        return false;
    }

    SDL_SetRenderDrawBlendMode(engine->renderer, SDL_BLENDMODE_BLEND);

    return true;
}

void Engine_Setup(struct EngineData *engine) {
    engine->player = Player_Create();
    engine->colorBuffer = ColorBuffer_Create(engine->renderer);
    Texture_LoadTextures();
}

void Engine_ProcessInput(struct EngineData *engine) {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        switch (event.type) {
        case SDL_QUIT:
            engine->isGameRunning = false;
            break;
        case SDL_KEYDOWN:
            if (event.key.keysym.sym == SDLK_ESCAPE) {
                engine->isGameRunning = false;
            } else {
                Player_ProcessInput(&engine->player, &event);
            }
            break;
        case SDL_KEYUP:
            Player_ProcessInput(&engine->player, &event);
            break;
        default:
            break;
        }
    }
}

void Engine_Update(struct EngineData *engine) {
    int timeTillNextFrame =
        FRAME_TIME_LENGHT - (SDL_GetTicks() - engine->ticksLastFrame);
    if (timeTillNextFrame > 0 && timeTillNextFrame <= FRAME_TIME_LENGHT) {
        SDL_Delay(timeTillNextFrame);
    }
    float deltaTime = (SDL_GetTicks() - engine->ticksLastFrame) / 1000.0f;
    engine->ticksLastFrame = SDL_GetTicks();

    Player_Move(&engine->player, deltaTime);
    Ray_CastAllRays(engine->rays, engine->player);
}

void Engine_Render(struct EngineData *engine) {
    ColorBuffer_Clear(engine->colorBuffer, RGBA(0, 0, 0, 255));

    Wall_Render3DProjection(engine->rays, engine->colorBuffer, engine->player);
    Sprite_RenderSpriteProjection(engine->colorBuffer, engine->rays,
                                  engine->player);

    // Rendering the mini map
    Map_Render(engine->colorBuffer, engine->renderer);
    Player_RenderOnMap(engine->player, engine->renderer);
    Ray_RenderRaysOnMap(engine->colorBuffer, engine->renderer, engine->rays,
                        engine->player);
    Sprite_RenderSpritesOnMap(engine->colorBuffer);

    ColorBuffer_Render(engine->colorBuffer, engine->renderer);
}
