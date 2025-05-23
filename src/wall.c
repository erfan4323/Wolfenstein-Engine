#include "texture.h"
#include "wall.h"

uint32_t ChangeColorIntensity(uint32_t color, float factor) {
    uint32_t a = (color >> 24) & 0xFF;
    uint32_t r = (color >> 16) & 0xFF;
    uint32_t g = (color >> 8) & 0xFF;
    uint32_t b = (color) & 0xFF;

    r = (uint32_t)(fminf(r * factor, 255.0f));
    g = (uint32_t)(fminf(g * factor, 255.0f));
    b = (uint32_t)(fminf(b * factor, 255.0f));

    return (uint32_t)RGBA(r, g, b, a);
}

void Wall_Render3DProjection(struct Ray rays[NUM_RAYS],
                             struct ColorBuffer *colorBuffer,
                             struct Player player) {
    for (int i = 0; i < NUM_RAYS; i++) {
        float perpDistance =
            rays[i].distance * cos(rays[i].rayAngle - player.rotationAngle);
        float distanceProjPlane = ((float)WINDOW_WIDTH / 2) / tan(FOV / 2);
        float projectedWallHeight =
            (TILE_SIZE / perpDistance) * distanceProjPlane;

        int wallStripHeight = (int)projectedWallHeight;

        int wallTopPixel = (WINDOW_HEIGHT / 2) - (wallStripHeight / 2);
        wallTopPixel = wallTopPixel < 0 ? 0 : wallTopPixel;

        int wallBottomPixel = (WINDOW_HEIGHT / 2) + (wallStripHeight / 2);
        wallBottomPixel =
            wallBottomPixel > WINDOW_HEIGHT ? WINDOW_HEIGHT : wallBottomPixel;

        for (int y = 0; y < wallTopPixel; y++) {
            ColorBuffer_DrawPixel(colorBuffer, i, y,
                                  (uint32_t)RGBA(130, 130, 130, 255));
        }

        int textureOffsetX;
        if (rays[i].hitData.wasHitVertical) {
            textureOffsetX = (int)rays[i].hitData.wallHitY % TILE_SIZE;
        } else {
            textureOffsetX = (int)rays[i].hitData.wallHitX % TILE_SIZE;
        }

        int textureNumber = rays[i].hitData.wallContent - 1;
        int textureWidth = wallTextures[textureNumber].width;
        int textureHeight = wallTextures[textureNumber].height;

        for (int y = wallTopPixel; y < wallBottomPixel; y++) {
            int distanceFromTop =
                y + (wallStripHeight / 2) - (WINDOW_HEIGHT / 2);
            int textureOffsetY =
                distanceFromTop * ((float)textureHeight / wallStripHeight);

            uint32_t texelColor =
                wallTextures[textureNumber]
                    .textureBuffer[(textureWidth * textureOffsetY) +
                                   textureOffsetX];

            if (rays[i].hitData.wasHitVertical) {
                texelColor = ChangeColorIntensity(texelColor, 0.7f);
            }

            ColorBuffer_DrawPixel(colorBuffer, i, y, texelColor);
        }

        for (int y = wallBottomPixel; y < WINDOW_HEIGHT; y++) {
            ColorBuffer_DrawPixel(colorBuffer, i, y,
                                  (uint32_t)RGBA(130, 130, 130, 255));
        }
    }
}
