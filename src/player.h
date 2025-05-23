#ifndef PLAYER_H
#define PLAYER_H

#include <SDL2/SDL.h>

struct Player {
    float x;
    float y;
    float width;
    float height;
    int turnDirection; // -1 for left, +1 for right
    int walkDirection; // -1 for backward, +1 for forward
    float rotationAngle;
    float walkSpeed;
    float turnSpeed;
};

struct Player Player_Create();

void Player_RenderOnMap(struct Player player, SDL_Renderer *renderer);

void Player_Move(struct Player *player, float deltaTime);

void Player_ProcessInput(struct Player *player, SDL_Event *event);

#endif
