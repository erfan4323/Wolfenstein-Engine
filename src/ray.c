#include "map.h"
#include "ray.h"
#include <limits.h>

void Ray_CastAllRays(struct Ray rays[NUM_RAYS], struct Player player) {
    for (int colId = 0; colId < NUM_RAYS; colId++) {
        float rayAngle = player.rotationAngle +
                         atan((colId - (float)NUM_RAYS / 2) / DIST_PROJ_PLANE);
        Ray_Cast(&rays[colId], player, rayAngle, colId);
    }
}

float NormalizeAngle(float angle) {
    angle = remainder(angle, TWO_PI);
    if (angle < 0) {
        angle += TWO_PI;
    }
    return angle;
}

struct RayDirection ComputeRayDirection(float rayAngle) {
    rayAngle = NormalizeAngle(rayAngle);
    struct RayDirection dir;

    dir.isRayFacingDown = rayAngle > 0 && rayAngle < PI;
    dir.isRayFacingUp = !dir.isRayFacingDown;

    dir.isRayFacingRight = rayAngle < 0.5f * PI || rayAngle > 1.5f * PI;
    dir.isRayFacingLeft = !dir.isRayFacingRight;

    return dir;
}

struct RayHitData HorizantalHit(struct Player player, float rayAngle) {
    struct RayDirection dir = ComputeRayDirection(rayAngle);

    float xIntercept, yIntercept;
    float xStep, yStep;

    struct RayHitData hitData = {false, false, 0, 0, 0};

    yIntercept = floor(player.y / TILE_SIZE) * TILE_SIZE;
    yIntercept += dir.isRayFacingDown ? TILE_SIZE : 0;

    xIntercept = player.x + (yIntercept - player.y) / tan(rayAngle);

    yStep = TILE_SIZE;
    yStep *= dir.isRayFacingUp ? -1 : 1;

    xStep = TILE_SIZE / tan(rayAngle);
    xStep *= (dir.isRayFacingLeft && xStep > 0) ? -1 : 1;
    xStep *= (dir.isRayFacingRight && xStep < 0) ? -1 : 1;

    float nextTouchX = xIntercept;
    float nextTouchY = yIntercept;

    while (Map_IsInsideMap(nextTouchX, nextTouchY)) {
        float xToCheck = nextTouchX;
        float yToCheck = nextTouchY + (dir.isRayFacingUp ? -1 : 0);

        if (Map_HasWallAt(xToCheck, yToCheck)) {
            int mapX = floor(xToCheck / TILE_SIZE);
            int mapY = floor(yToCheck / TILE_SIZE);
            hitData.wallHitX = nextTouchX;
            hitData.wallHitY = nextTouchY;
            hitData.foundWallHit = true;
            hitData.wasHitVertical = false;
            hitData.wallContent = Map_GetValueAt(mapY, mapX);

            break;
        } else {
            nextTouchX += xStep;
            nextTouchY += yStep;
        }
    }

    return hitData;
}

struct RayHitData VerticalHit(struct Player player, float rayAngle) {
    struct RayDirection dir = ComputeRayDirection(rayAngle);

    float xIntercept, yIntercept;
    float xStep, yStep;

    struct RayHitData hitData = {false, false, 0, 0, 0};

    xIntercept = floor(player.x / TILE_SIZE) * TILE_SIZE;
    xIntercept += dir.isRayFacingRight ? TILE_SIZE : 0;

    yIntercept = player.y + (xIntercept - player.x) * tan(rayAngle);

    xStep = TILE_SIZE;
    xStep *= dir.isRayFacingLeft ? -1 : 1;

    yStep = TILE_SIZE * tan(rayAngle);
    yStep *= (dir.isRayFacingUp && yStep > 0) ? -1 : 1;
    yStep *= (dir.isRayFacingDown && yStep < 0) ? -1 : 1;

    float nextTouchX = xIntercept;
    float nextTouchY = yIntercept;

    while (Map_IsInsideMap(nextTouchX, nextTouchY)) {
        float xToCheck = nextTouchX + (dir.isRayFacingLeft ? -1 : 0);
        float yToCheck = nextTouchY;

        if (Map_HasWallAt(xToCheck, yToCheck)) {
            int mapX = floor(xToCheck / TILE_SIZE);
            int mapY = floor(yToCheck / TILE_SIZE);
            hitData.wallHitX = nextTouchX;
            hitData.wallHitY = nextTouchY;
            hitData.foundWallHit = true;
            hitData.wasHitVertical = true;
            hitData.wallContent = Map_GetValueAt(mapY, mapX);
            break;
        } else {
            nextTouchX += xStep;
            nextTouchY += yStep;
        }
    }

    return hitData;
}
float calculateDistance(float x1, float y1, float x2, float y2) {
    float dx = x2 - x1;
    float dy = y2 - y1;
    return sqrtf(dx * dx + dy * dy);
}

void Ray_Cast(struct Ray *ray, struct Player player, float rayAngle,
              int colId) {
    rayAngle = NormalizeAngle(rayAngle);
    struct RayHitData horzData = HorizantalHit(player, rayAngle);
    struct RayHitData vertData = VerticalHit(player, rayAngle);

    float horzDistance =
        horzData.foundWallHit
            ? calculateDistance(player.x, player.y, horzData.wallHitX,
                                horzData.wallHitY)
            : INT_MAX;

    float vertDistance =
        vertData.foundWallHit
            ? calculateDistance(player.x, player.y, vertData.wallHitX,
                                vertData.wallHitY)
            : INT_MAX;

    bool isHorzLessThanVert = (horzDistance < vertDistance);
    ray->hitData.wallContent =
        isHorzLessThanVert ? horzData.wallContent : vertData.wallContent;
    ray->hitData.wallHitX =
        isHorzLessThanVert ? horzData.wallHitX : vertData.wallHitX;
    ray->hitData.wallHitY =
        isHorzLessThanVert ? horzData.wallHitY : vertData.wallHitY;
    ray->hitData.wasHitVertical = !isHorzLessThanVert;
    ray->distance = isHorzLessThanVert ? horzDistance : vertDistance;
    ray->rayAngle = rayAngle;
    ray->direction = ComputeRayDirection(rayAngle);
}

void Ray_RenderRays(struct ColorBuffer *cb, SDL_Renderer *renderer,
                    struct Ray rays[NUM_RAYS], struct Player player) {
    for (int i = 0; i < NUM_RAYS; i++) {
        ColorBuffer_DrawLine(cb, MINI_MAP_SCALE_FACTOR * player.x,
                             MINI_MAP_SCALE_FACTOR * player.y,
                             MINI_MAP_SCALE_FACTOR * rays[i].hitData.wallHitX,
                             MINI_MAP_SCALE_FACTOR * rays[i].hitData.wallHitY,
                             RGBA(0, 0, 255, 255));
    }
}
