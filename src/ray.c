#include "map.h"
#include "ray.h"
#include "texture.h"
#include <limits.h>

void Ray_CastAllRays(struct Ray rays[NUM_RAYS], struct Player player) {
    float rayAngle = player.rotationAngle - (FOV / 2);

    for (int colId = 0; colId < NUM_RAYS; colId++) {
        Ray_Cast(&rays[colId], player, rayAngle, colId);
        rayAngle += FOV / NUM_RAYS;
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

    while (nextTouchX >= 0 && nextTouchX <= WINDOW_WIDTH && nextTouchY >= 0 &&
           nextTouchY <= WINDOW_HEIGHT) {
        float xToCheck = nextTouchX;
        float yToCheck = nextTouchY + (dir.isRayFacingUp ? -1 : 0);

        if (Map_HasWallAt(xToCheck, yToCheck)) {
            int mapX = floor(xToCheck / TILE_SIZE);
            int mapY = floor(yToCheck / TILE_SIZE);
            hitData.wallHitX = nextTouchX;
            hitData.wallHitY = nextTouchY;
            hitData.foundWallHit = true;
            hitData.wasHitVertical = false;
            hitData.wallContent = MAP[mapY][mapX];

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

    while (nextTouchX >= 0 && nextTouchX <= WINDOW_WIDTH && nextTouchY >= 0 &&
           nextTouchY <= WINDOW_HEIGHT) {
        float xToCheck = nextTouchX + (dir.isRayFacingLeft ? -1 : 0);
        float yToCheck = nextTouchY;

        if (Map_HasWallAt(xToCheck, yToCheck)) {
            int mapX = floor(xToCheck / TILE_SIZE);
            int mapY = floor(yToCheck / TILE_SIZE);
            hitData.wallHitX = nextTouchX;
            hitData.wallHitY = nextTouchY;
            hitData.foundWallHit = true;
            hitData.wasHitVertical = true;
            hitData.wallContent = MAP[mapY][mapX];
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

void Ray_RenderRays(SDL_Renderer *renderer, struct Ray rays[NUM_RAYS],
                    struct Player player) {
    SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
    for (int i = 0; i < NUM_RAYS; i++) {
        SDL_RenderDrawLine(renderer, MINI_MAP_SCALE_FACTOR * player.x,
                           MINI_MAP_SCALE_FACTOR * player.y,
                           MINI_MAP_SCALE_FACTOR * rays[i].hitData.wallHitX,
                           MINI_MAP_SCALE_FACTOR * rays[i].hitData.wallHitY);
    }
}

void Ray_Render3DProjection(struct Ray rays[NUM_RAYS],
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
            int bufferIndex = (WINDOW_WIDTH * y) + i;
            colorBuffer->pixels[bufferIndex] = RGBA(130, 130, 130, 255);
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

            int bufferIndex = (WINDOW_WIDTH * y) + i;
            colorBuffer->pixels[bufferIndex] = texelColor;
        }

        for (int y = wallBottomPixel; y < WINDOW_HEIGHT; y++) {
            int bufferIndex = (WINDOW_WIDTH * y) + i;
            colorBuffer->pixels[bufferIndex] = RGBA(90, 90, 90, 255);
        }
    }
}
