#include <iostream>
#include <ncurses.h>

#include "GameState.h"
#include "constants.h"

# ifndef RENDER
# define RENDER

class Render{
  public:
  Render(GameState *gamestate, int map_scale) : gs(gamestate), map_scale(map_scale) {
    if(map_scale % 2 != 0){
      throw std::invalid_argument("map scale must be even!");
    }
    initscr();
    //cbreak();
    //noecho();
    curs_set(0);  // hide cursor
    start_color();

    int map_width = gs->get_width() * map_scale, map_height = gs->get_height() * (map_scale / 2);

    map_window = newwin(map_height, map_width, 1, 1);
    refresh();

    init_pair(IMMUNE_WALL + 1, COLOR_BLUE, COLOR_BLUE);
    init_pair(WALL + 1, COLOR_BLACK, COLOR_BLACK);
    init_pair(EMPTY + 1, COLOR_WHITE, COLOR_WHITE);
    init_pair(SNAKE_BODY + 1, COLOR_YELLOW, COLOR_YELLOW);
    init_pair(SNAKE_HEAD + 1, COLOR_MAGENTA, COLOR_MAGENTA);

    wbkgd(stdscr, COLOR_PAIR(EMPTY + 1));
    refresh();

  };
  ~Render(){
    endwin();
  }

  void draw_map() {
    //wborder(map_window, 0, 0, 0, 0, 0, 0, 0, 0);

    for(int y = 0; y < gs->get_height(); y++){
      for(int x = 0; x < gs->get_width(); x++){
        for(int x_scalefactor = 0; x_scalefactor < map_scale; x_scalefactor++){
          for(int y_scalefactor = 0; y_scalefactor < map_scale/2; y_scalefactor++){
            wmove(map_window, y * (map_scale/2) + y_scalefactor, x * map_scale + x_scalefactor);

            int map_value = gs->get_coord(y, x);

            switch (map_value)
            {
            case IMMUNE_WALL:
              // ACS_CKBOARD
              waddch(map_window, ' ' | COLOR_PAIR(IMMUNE_WALL + 1));
              break;
            
            case WALL:
              waddch(map_window, ' ' | COLOR_PAIR(WALL + 1));
              break;
            
            case EMPTY:
              waddch(map_window, ' ' | COLOR_PAIR(EMPTY + 1));
              break;
            
            case SNAKE_BODY:
              waddch(map_window, ' ' | COLOR_PAIR(SNAKE_BODY + 1));
              break;

            case SNAKE_HEAD:
              waddch(map_window, ' ' | COLOR_PAIR(SNAKE_HEAD + 1));
              break;
            }

            wrefresh(map_window);
          }
        }
      }
    }
  };

  void draw_map_test(){
    int color = 1;
    for(int y = 0; y < gs->get_height(); y++){
      for(int x = 0; x < gs->get_width(); x++){
        for(int x_scalefactor = 0; x_scalefactor < map_scale; x_scalefactor++){
          for(int y_scalefactor = 0; y_scalefactor < map_scale / 2; y_scalefactor++){
            wmove(map_window, y * (map_scale/2) + y_scalefactor, x * map_scale + x_scalefactor);

            waddch(map_window, ' ' | COLOR_PAIR(color));

            wrefresh(map_window);
          }
        }

        color = ++color % 2;
      }
    }
  }

  private:
    GameState *gs;
    WINDOW *map_window;
    int map_scale;
};

# endif