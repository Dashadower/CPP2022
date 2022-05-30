#include <iostream>
#include <ncurses.h>
#include "GameState.h"
#include "render.h"

int main(void){
  GameState gs(21, 10);
  Render render(&gs, 2);
  render.draw_map();
  //gs.print_map();
  getch();
  return 0;
}
