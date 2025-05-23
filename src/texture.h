#ifndef TEXTURE_H
#define TEXTURE_H
#include "constants.h"
#include <SDL2/SDL.h>

extern const char *textureFileNames[NUM_TEXTURES];
extern struct Texture textures[NUM_TEXTURES];

struct Texture {
    int width;
    int height;
    uint32_t *textureBuffer;
};

void Texture_LoadTextures();
void Texture_FreeTextures();

#endif
