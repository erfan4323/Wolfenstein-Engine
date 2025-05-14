#include "map.h"

void Map_Render(SDL_Renderer *renderer) {
    for (int i = 0; i < MAP_NUM_ROWS; i++) {
        for (int j = 0; j < MAP_NUM_COLS; j++) {
            int tileX = j * TILE_SIZE;
            int tileY = i * TILE_SIZE;
            int tileColor = MAP[i][j] != 0 ? 255 : 0;
            SDL_SetRenderDrawColor(renderer, tileColor, tileColor, tileColor,
                                   255);
            SDL_Rect mapTileRect = {MINI_MAP_SCALE_FACTOR * tileX,
                                    MINI_MAP_SCALE_FACTOR * tileY,
                                    MINI_MAP_SCALE_FACTOR * TILE_SIZE,
                                    MINI_MAP_SCALE_FACTOR * TILE_SIZE};
            SDL_RenderFillRect(renderer, &mapTileRect);
        }
    }
}

int Map_HasWallAt(float x, float y) {
    if (x < 0 || x > WINDOW_WIDTH || y < 0 || y > WINDOW_HEIGHT) {
        return 1;
    }

    int mapGridIndexX = floor(x / TILE_SIZE);
    int mapGridIndexY = floor(y / TILE_SIZE);

    return MAP[mapGridIndexY][mapGridIndexX] != 0;
}
