#include "constants.h"
#include "map.h"
#include "player.h"
#include "utils.h"

struct Player Player_Create() {
    struct Player player = {WINDOW_WIDTH / 2.0f,
                            WINDOW_HEIGHT / 2.0f,
                            10.0f,
                            10.0f,
                            0,
                            0,
                            PI / 2.0f,
                            200.0f,
                            DEGTORAD(70.0f)};

    return player;
}

void Player_RenderOnMap(struct Player player, SDL_Renderer *renderer) {
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_Rect playerRect = {(int)(MINI_MAP_SCALE_FACTOR * player.x),
                           (int)(MINI_MAP_SCALE_FACTOR * player.y),
                           (int)(MINI_MAP_SCALE_FACTOR * player.width),
                           (int)(MINI_MAP_SCALE_FACTOR * player.height)};
    SDL_RenderFillRect(renderer, &playerRect);

    SDL_RenderDrawLine(
        renderer, MINI_MAP_SCALE_FACTOR * player.x,
        MINI_MAP_SCALE_FACTOR * player.y,
        MINI_MAP_SCALE_FACTOR * player.x + cos(player.rotationAngle) * 40,
        MINI_MAP_SCALE_FACTOR * player.y + sin(player.rotationAngle) * 40);
}

void Player_Move(struct Player *player, float deltaTime) {
    player->rotationAngle +=
        player->turnDirection * player->turnSpeed * deltaTime;
    player->rotationAngle = NormalizeAngle(player->rotationAngle);

    if (player->walkDirection != 0) {

        float moveStep = player->walkDirection * player->walkSpeed * deltaTime;

        float newPlayerX = player->x + cos(player->rotationAngle) * moveStep;
        float newPlayerY = player->y + sin(player->rotationAngle) * moveStep;

        if (!Map_HasWallAt(newPlayerX, newPlayerY)) {

            player->x = newPlayerX;
            player->y = newPlayerY;
        }
    }
}

void Player_ProcessInput(struct Player *player, SDL_Event *event) {
    if (event->type == SDL_KEYDOWN) {
        switch (event->key.keysym.sym) {
        case SDLK_UP:
            player->walkDirection = 1;
            break;
        case SDLK_DOWN:
            player->walkDirection = -1;
            break;
        case SDLK_RIGHT:
            player->turnDirection = 1;
            break;
        case SDLK_LEFT:
            player->turnDirection = -1;
            break;
        default:
            break;
        }
    } else if (event->type == SDL_KEYUP) {
        switch (event->key.keysym.sym) {
        case SDLK_UP:
        case SDLK_DOWN:
            player->walkDirection = 0;
            break;
        case SDLK_RIGHT:
        case SDLK_LEFT:
            player->turnDirection = 0;
            break;
        default:
            break;
        }
    }
}
