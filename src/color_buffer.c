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
    for (int i = 0; i < cb->height * cb->width; i++) {
        cb->pixels[i] = color;
    }
}

void ColorBuffer_Render(struct ColorBuffer *cb, SDL_Renderer *renderer) {
    SDL_UpdateTexture(cb->texture, NULL, cb->pixels,
                      cb->width * sizeof(uint32_t));
    SDL_RenderCopy(renderer, cb->texture, NULL, NULL);
    SDL_RenderPresent(renderer);
}

void ColorBuffer_DrawPixel(struct ColorBuffer *cb, int x, int y,
                           uint32_t color) {
    int bufferIndex = (WINDOW_WIDTH * y) + x;
    cb->pixels[bufferIndex] = color;
}

void ColorBuffer_DrawRect(struct ColorBuffer *cb, int x, int y, int width,
                          int height, uint32_t color) {
    for (int i = x; i <= (x + width); i++) {
        for (int j = y; j <= (y + height); j++) {
            ColorBuffer_DrawPixel(cb, i, j, color);
        }
    }
}

void ColorBuffer_DrawLine(struct ColorBuffer *cb, int x0, int y0, int x1,
                          int y1, uint32_t color) {
    int deltaX = (x1 - x0);
    int deltaY = (y1 - y0);

    int sideLength = abs(deltaX) > abs(deltaY) ? abs(deltaX) : abs(deltaY);

    float xIncrement = deltaX / (float)sideLength;
    float yIncrement = deltaY / (float)sideLength;

    float currentX = x0;
    float currentY = y0;

    for (int i = 0; i < sideLength; i++) {
        ColorBuffer_DrawPixel(cb, round(currentX), round(currentY), color);
        currentX += xIncrement;
        currentY += yIncrement;
    }
}
