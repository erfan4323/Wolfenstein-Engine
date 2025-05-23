#ifndef CONSTANTS_H
#define CONSTANTS_H

#include <stdbool.h>

#define PI 3.14159265358979323846
#define TWO_PI 6.28318530717958647692
#define DEGTORAD(x) (x * (PI / 180))

#define TILE_SIZE 64
#define MAP_NUM_ROWS 13
#define MAP_NUM_COLS 20

#define MINI_MAP_SCALE_FACTOR 0.3

#define WINDOW_WIDTH 1400
#define WINDOW_HEIGHT 900

#define NUM_TEXTURES 9

#define FOV DEGTORAD(60)

#define DIST_PROJ_PLANE ((WINDOW_WIDTH / 2) / tan(FOV / 2))

#define NUM_RAYS WINDOW_WIDTH

#define FPS 30
#define FRAME_TIME_LENGHT (1000 / FPS)

#define RGB(r, g, b) (0xFF000000 | ((r) << 16) | ((g) << 8) | (b))
#define RGBA(r, g, b, a) (((a) << 24) | ((r) << 16) | ((g) << 8) | (b))

#endif
