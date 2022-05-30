#include <iostream>
#include <ncurses.h>
#include "GameState.h"
# ifndef RENDER
# define RENDER

class Render{
  public:
  Render(GameState *gamestate) : gs(gamestate) {

  };
  private:
    GameState *gs;
}

# endif