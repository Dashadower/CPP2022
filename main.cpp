#include <iostream>
#include <ncurses.h>

#include "game_state.h"
#include "render.h"
#include "constants.h"

#define TICK 2

int main(void){
  GameState gs(25, 25);
  Render render(&gs, 2);
  bool keep_running = true;
  while(keep_running){
    gs.tick();
    render.draw_map();
    halfdelay(TICK);
    int key = getch();
    switch (key)
    {
    case KEY_UP:
      gs.set_direction(UP);
      break;
    
    case KEY_DOWN:
      gs.set_direction(DOWN);
      break;

    case KEY_LEFT:
      gs.set_direction(LEFT);
      break;

    case KEY_RIGHT:
      gs.set_direction(RIGHT);
      break;

    case 'q':
      keep_running = false;
      break;

    }
  }
  
  //gs.print_map();
  getch();
  return 0;
}
