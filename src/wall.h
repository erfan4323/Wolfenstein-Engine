#ifndef WALL_H
#define WALL_H

#include "color_buffer.h"
#include "constants.h"
#include "player.h"
#include "ray.h"

void Wall_Render3DProjection(struct Ray rays[NUM_RAYS],
                             struct ColorBuffer *colorBuffer,
                             struct Player player);

#endif
