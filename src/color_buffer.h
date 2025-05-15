#ifndef COLOR_BUFFER_H
#define COLOR_BUFFER_H

#include <SDL2/SDL.h>

struct ColorBuffer {
    Uint32 *pixels;
    SDL_Texture *texture;
    int width;
    int height;
};

struct ColorBuffer *ColorBuffer_Create(SDL_Renderer *renderer);
void ColorBuffer_Destroy(struct ColorBuffer *cb);
void ColorBuffer_Clear(struct ColorBuffer *cb, Uint32 color);
void ColorBuffer_Render(struct ColorBuffer *cb, SDL_Renderer *renderer);

#endif
