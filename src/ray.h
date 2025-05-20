#ifndef RAY_H
#define RAY_H

#include "color_buffer.h"
#include "constants.h"
#include "player.h"

struct RayHitData {
    bool foundWallHit;
    bool wasHitVertical;
    float wallHitX;
    float wallHitY;
    int wallContent;
};

struct RayDirection {
    int isRayFacingUp;
    int isRayFacingDown;
    int isRayFacingLeft;
    int isRayFacingRight;
};

struct Ray {
    float rayAngle;
    float distance;
    struct RayDirection direction;
    struct RayHitData hitData;
};

void Ray_CastAllRays(struct Ray rays[NUM_RAYS], struct Player player);

void Ray_Cast(struct Ray *ray, struct Player player, float rayAngle, int colId);

void Ray_RenderRays(SDL_Renderer *renderer, struct Ray rays[NUM_RAYS],
                    struct Player player);

void Ray_Render3DProjection(struct Ray rays[NUM_RAYS],
                            struct ColorBuffer *colorBuffer,
                            struct Player player);

#endif
