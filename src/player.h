#ifndef PLAYER_H
#define PLAYER_H

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

struct Player CreatePlayer();

#endif
