#ifndef COLOR_BUFFER_H
#define COLOR_BUFFER_H

#include <SDL2/SDL.h>

struct ColorBuffer {
    uint32_t *pixels;
    SDL_Texture *texture;
    int width;
    int height;
};

struct ColorBuffer *ColorBuffer_Create(SDL_Renderer *renderer);
void ColorBuffer_Destroy(struct ColorBuffer *cb);
void ColorBuffer_Clear(struct ColorBuffer *cb, uint32_t color);
void ColorBuffer_Render(struct ColorBuffer *cb, SDL_Renderer *renderer);
void ColorBuffer_DrawPixel(struct ColorBuffer *cb, int x, int y,
                           uint32_t color);
void ColorBuffer_DrawRect(struct ColorBuffer *cb, int x, int y, int width,
                          int height, uint32_t color);
void ColorBuffer_DrawLine(struct ColorBuffer *cb, int x0, int y0, int x1,
                          int y1, uint32_t color);
#endif
