#ifndef SPRITE_H
#define SPRITE_H

#include "color_buffer.h"
#include "constants.h"
#include "player.h"
#include "ray.h"

struct Sprite {
    float x;
    float y;
    float distance;
    float angle;
    bool visible;
    int texture;
};

extern struct Sprite sprites[NUM_SPRITES];

void Sprite_RenderSpritesOnMap(struct ColorBuffer *cb);
void Sprite_RenderSpriteProjection(struct ColorBuffer *cb,
                                   struct Ray rays[NUM_RAYS],
                                   struct Player player);

#endif
