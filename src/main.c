#define SDL_MAIN_HANDLED
#include <SDL2/SDL.h>

#include "engine.h"

int main(int argc, char **argv) {
    struct EngineData *engine = Engine_Create();

    engine->isGameRunning = Engine_InitializeWindow(engine);
    Engine_Setup(engine);

    while (engine->isGameRunning) {
        Engine_ProcessInput(engine);
        Engine_Update(engine);
        Engine_Render(engine);
    }

    Engine_Destroy(engine);

    return 0;
}
