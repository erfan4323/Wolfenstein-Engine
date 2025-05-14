#include "player.h"
#include "constants.h"

struct Player CreatePlayer() {
      struct Player player = {
          WINDOW_WIDTH / 2.0f,
          WINDOW_HEIGHT / 2.0f,
          5.0f,
          5.0f,
          0,
          0,
          PI / 2.0f,
          100.0f,
          DEGTORAD(45.0f)
      };

      return player;
}
