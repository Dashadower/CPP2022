#ifndef LEVELS
#define LEVELS

#include "constants.h"

struct Level {
  int level_number;
  int goal_b;
  int goal_growth;
  int goal_poison;
  int goal_gate;
  int **game_map;
};


#endif