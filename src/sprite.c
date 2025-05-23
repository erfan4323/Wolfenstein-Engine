#include "sprite.h"
#include "texture.h"
#include "utils.h"

struct Sprite sprites[NUM_SPRITES] = {
    {640, 630, 0, 0, false, 9},  // barrel
    {660, 690, 0, 0, false, 9},  // barrel
    {250, 600, 0, 0, false, 11}, // table
    {250, 600, 0, 0, false, 10}, // light
    {300, 400, 0, 0, false, 12}, // guard
    {90, 100, 0, 0, false, 13}   // armor
};

void Sprite_RenderSpritesOnMap(struct ColorBuffer *cb) {
    for (int i = 0; i < NUM_SPRITES; i++) {
        ColorBuffer_DrawRect(cb, MINI_MAP_SCALE_FACTOR * sprites[i].x,
                             MINI_MAP_SCALE_FACTOR * sprites[i].y, 2, 2,
                             sprites[i].visible ? RGBA(255, 252, 127, 255)
                                                : RGBA(162, 162, 162, 255));
    }
}
void Sprite_RenderSpriteProjection(struct ColorBuffer *cb,
                                   struct Ray rays[NUM_RAYS],
                                   struct Player player) {
    struct Sprite visibleSprites[NUM_SPRITES];
    int numVisibleSprites = 0;

    for (int i = 0; i < NUM_SPRITES; i++) {
        float angleSpritePlayer =
            player.rotationAngle -
            atan2(sprites[i].y - player.y, sprites[i].x - player.x);

        if (angleSpritePlayer > PI) {
            angleSpritePlayer -= TWO_PI;
        }

        if (angleSpritePlayer < -PI) {
            angleSpritePlayer += TWO_PI;
        }

        angleSpritePlayer = fabs(angleSpritePlayer);

        const float EPSILON = 0.2f;

        if (angleSpritePlayer < (FOV / 2) + EPSILON) {
            sprites[i].visible = true;
            sprites[i].angle = angleSpritePlayer;
            sprites[i].distance = calculateDistance(sprites[i].x, sprites[i].y,
                                                    player.x, player.y);
            visibleSprites[numVisibleSprites] = sprites[i];
            numVisibleSprites++;
        } else {
            sprites[i].visible = false;
        }
    }

    for (int i = 0; i < numVisibleSprites - 1; i++) {
        for (int j = i; j < numVisibleSprites; j++) {
            if (visibleSprites[i].distance < visibleSprites[j].distance) {
                struct Sprite temp = visibleSprites[i];
                visibleSprites[i] = visibleSprites[j];
                visibleSprites[j] = temp;
            }
        }
    }

    for (int j = 0; j < numVisibleSprites; j++) {
        struct Sprite sprite = visibleSprites[j];

        float perpDistance = sprite.distance * cos(sprite.angle);

        float spriteHeight = (TILE_SIZE / perpDistance) * DIST_PROJ_PLANE;
        float spriteWidth = spriteHeight;

        float spriteTopY = (WINDOW_HEIGHT / 2) - (spriteHeight / 2);
        spriteTopY = (spriteTopY < 0) ? 0 : spriteTopY;

        float spriteBottomY = (WINDOW_HEIGHT / 2) + (spriteHeight / 2);
        spriteBottomY =
            (spriteBottomY > WINDOW_HEIGHT) ? WINDOW_HEIGHT : spriteBottomY;

        float spriteAngle = atan2(sprite.y - player.y, sprite.x - player.x) -
                            player.rotationAngle;
        float spriteScreenPosX = tan(spriteAngle) * DIST_PROJ_PLANE;

        float spriteLeftX =
            (WINDOW_WIDTH / 2) + spriteScreenPosX - (spriteWidth / 2);
        float spriteRightX = spriteLeftX + spriteWidth;

        struct Texture currentTexture = textures[sprite.texture];

        for (int x = spriteLeftX; x < spriteRightX; x++) {
            float texelWidth = ((float)currentTexture.width / spriteWidth);
            int textureOffsetX = (x - spriteLeftX) * texelWidth;

            for (int y = spriteTopY; y < spriteBottomY; y++) {
                if (x > 0 && x < WINDOW_WIDTH && y > 0 && y < WINDOW_HEIGHT) {
                    int distanceFromTop =
                        y + (spriteHeight / 2) - (WINDOW_HEIGHT / 2);
                    int textureOffsetY =
                        distanceFromTop *
                        ((float)currentTexture.height / spriteHeight);

                    int bufferIndex = (currentTexture.width * textureOffsetY) +
                                      textureOffsetX;
                    uint32_t texelColor =
                        currentTexture.textureBuffer[bufferIndex];

                    if (sprite.distance < rays[x].distance &&
                        texelColor != RGBA(255, 0, 255, 255)) {
                        ColorBuffer_DrawPixel(cb, x, y, texelColor);
                    }
                }
            }
        }
    }
}
