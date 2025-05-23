#include "constants.h"
#include "texture.h"
#include <SDL2/SDL.h>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

const char *textureFileNames[NUM_TEXTURES] = {
    "images/walls/BRICK_2B.PNG",    "images/walls/BRICK_4A.PNG",
    "images/walls/CONCRETE_3C.PNG", "images/walls/DOOR_1A.PNG",
    "images/walls/HEDGE_1A.PNG",    "images/walls/WOOD_1C.PNG",
    "images/walls/SLIME_1A.PNG",    "images/walls/WARN_1A.PNG",
    "images/walls/PIPES_1A.PNG",    "images/sprites/barrel.png",
    "images/sprites/light.png",     "images/sprites/table.png",
    "images/sprites/guard.png",     "images/sprites/armor.png"};

struct Texture textures[NUM_TEXTURES];

void Texture_LoadTextures() {
    for (int i = 0; i < NUM_TEXTURES; i++) {
        int width, height, channels;
        unsigned char *pixels =
            stbi_load(textureFileNames[i], &width, &height, &channels, 4);

        if (!pixels) {
            fprintf(stderr, "Failed to load image: %s\n",
                    stbi_failure_reason());
        }

        textures[i].width = width;
        textures[i].height = height;
        textures[i].textureBuffer = (uint32_t *)pixels;
    }
}

void Texture_FreeTextures() {
    for (int i = 0; i < NUM_TEXTURES; i++) {
        stbi_image_free(textures[i].textureBuffer);
    }
}
