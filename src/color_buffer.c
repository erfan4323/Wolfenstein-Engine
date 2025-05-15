#include "color_buffer.h"
#include "constants.h"

struct ColorBuffer *ColorBuffer_Create(SDL_Renderer *renderer) {
    struct ColorBuffer *cb =
        (struct ColorBuffer *)malloc(sizeof(struct ColorBuffer));

    cb->width = WINDOW_WIDTH;
    cb->height = WINDOW_HEIGHT;
    cb->pixels = (Uint32 *)malloc(sizeof(Uint32) * (Uint32)WINDOW_WIDTH *
                                  (Uint32)WINDOW_HEIGHT);
    cb->texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ABGR8888,
                                    SDL_TEXTUREACCESS_STREAMING, WINDOW_WIDTH,
                                    WINDOW_HEIGHT);
    return cb;
}

void ColorBuffer_Destroy(struct ColorBuffer *cb) {
    if (!cb)
        return;
    free(cb->pixels);
    SDL_DestroyTexture(cb->texture);
    free(cb);
}

void ColorBuffer_Clear(struct ColorBuffer *cb, Uint32 color) {
    for (int x = 0; x < cb->width; x++) {
        for (int y = 0; y < cb->height; y++) {
            int bufferIndex = (cb->width * y) + x;
            cb->pixels[bufferIndex] = color;
        }
    }
}

void ColorBuffer_Render(struct ColorBuffer *cb, SDL_Renderer *renderer) {
    SDL_UpdateTexture(cb->texture, NULL, cb->pixels,
                      cb->width * sizeof(Uint32));
    SDL_RenderCopy(renderer, cb->texture, NULL, NULL);
}
