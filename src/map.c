#include "color_buffer.h"
#include "map.h"

void Map_Render(struct ColorBuffer *cb, SDL_Renderer *renderer) {
    for (int i = 0; i < MAP_NUM_ROWS; i++) {
        for (int j = 0; j < MAP_NUM_COLS; j++) {
            int tileX = j * TILE_SIZE;
            int tileY = i * TILE_SIZE;
            uint32_t tileColor =
                MAP[i][j] != 0 ? RGBA(255, 255, 255, 255) : RGBA(0, 0, 0, 255);
            ColorBuffer_DrawRect(cb, (int)(MINI_MAP_SCALE_FACTOR * tileX),
                                 (int)(MINI_MAP_SCALE_FACTOR * tileY),
                                 (int)(MINI_MAP_SCALE_FACTOR * TILE_SIZE),
                                 (int)(MINI_MAP_SCALE_FACTOR * TILE_SIZE),
                                 tileColor);
        }
    }
}

bool Map_HasWallAt(float x, float y) {
    if (x < 0 || x >= MAP_NUM_COLS * TILE_SIZE || y < 0 ||
        y >= MAP_NUM_ROWS * TILE_SIZE) {
        return true;
    }

    int mapGridIndexX = floor(x / TILE_SIZE);
    int mapGridIndexY = floor(y / TILE_SIZE);

    return MAP[mapGridIndexY][mapGridIndexX] != 0;
}

int Map_GetValueAt(int x, int y) { return MAP[x][y]; }

int Map_IsInsideMap(int x, int y) {
    return x >= 0 && x <= MAP_NUM_COLS * TILE_SIZE && y >= 0 &&
           y <= MAP_NUM_ROWS * TILE_SIZE;
}
