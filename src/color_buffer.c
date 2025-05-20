#include "color_buffer.h"
#include "constants.h"

struct ColorBuffer *ColorBuffer_Create(SDL_Renderer *renderer) {
    struct ColorBuffer *cb =
        (struct ColorBuffer *)malloc(sizeof(struct ColorBuffer));

    cb->width = WINDOW_WIDTH;
    cb->height = WINDOW_HEIGHT;
    cb->pixels = (uint32_t *)malloc(sizeof(uint32_t) * (uint32_t)WINDOW_WIDTH *
                                    (uint32_t)WINDOW_HEIGHT);
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

void ColorBuffer_Clear(struct ColorBuffer *cb, uint32_t color) {
    for (int x = 0; x < cb->width; x++) {
        for (int y = 0; y < cb->height; y++) {
            int bufferIndex = (cb->width * y) + x;
            cb->pixels[bufferIndex] = color;
        }
    }
}

void ColorBuffer_Render(struct ColorBuffer *cb, SDL_Renderer *renderer) {
    SDL_UpdateTexture(cb->texture, NULL, cb->pixels,
                      cb->width * sizeof(uint32_t));
    SDL_RenderCopy(renderer, cb->texture, NULL, NULL);
}
