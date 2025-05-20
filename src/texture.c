#include "constants.h"
#include "texture.h"
#include <SDL2/SDL.h>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

const char *textureFileNames[NUM_TEXTURES] = {
    "images/BRICK_2B.PNG", "images/BRICK_4A.PNG", "images/CONCRETE_3C.PNG",
    "images/DOOR_1A.PNG",  "images/HEDGE_1A.PNG", "images/WOOD_1C.PNG",
    "images/SLIME_1A.PNG", "images/WARN_1A.PNG",  "images/PIPES_1A.PNG"};

struct Texture wallTextures[NUM_TEXTURES];

void Texture_LoadWalls() {
    for (int i = 0; i < NUM_TEXTURES; i++) {
        int width, height, channels;
        unsigned char *pixels =
            stbi_load(textureFileNames[i], &width, &height, &channels, 4);

        if (!pixels) {
            fprintf(stderr, "Failed to load image: %s\n",
                    stbi_failure_reason());
        }

        wallTextures[i].width = width;
        wallTextures[i].height = height;
        wallTextures[i].textureBuffer = (uint32_t *)pixels;
    }
}

void Texture_FreeWallTextures() {
    for (int i = 0; i < NUM_TEXTURES; i++) {
        stbi_image_free(wallTextures[i].textureBuffer);
    }
}
