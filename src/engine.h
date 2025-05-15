#ifndef ENGINE_H
#define ENGINE_H

#include "color_buffer.h"
#include "player.h"
#include "ray.h"
#include <SDL2/SDL.h>
#include <stdbool.h>

struct EngineData {
    SDL_Window *window;
    SDL_Renderer *renderer;

    bool isGameRunning;
    int ticksLastFrame;

    struct Player player;
    struct Ray rays[NUM_RAYS];
    struct ColorBuffer *colorBuffer;
};

struct EngineData *Engine_Create(void);
void Engine_Destroy(struct EngineData *engine);

bool Engine_InitializeWindow(struct EngineData *engine);
void Engine_Setup(struct EngineData *engine);
void Engine_ProcessInput(struct EngineData *engine);
void Engine_Update(struct EngineData *engine);
void Engine_Render(struct EngineData *engine);

#endif
