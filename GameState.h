#include <iostream>
#include "constants.h"


# ifndef GAMESTATE
# define GAMESTATE

class GameState{
  private:
    int width, height;
    int **game_map;

  public:
    GameState(int width, int height) : width(width), height(height) {
      game_map = new int *[height];
      for(int i = 0; i < height; i++){
        game_map[i] = new int[width];
      }

      // build walls and corners
      for(int x = 0; x < width; x++){
        game_map[0][x] = 1;
        game_map[height - 1][x] = WALL;
      }
      for(int y = 0; y < height; y++){
        game_map[y][0] = 1;
        game_map[y][width - 1] = WALL;
      }
      game_map[0][0] = IMMUNE_WALL;
      game_map[0][width - 1] = IMMUNE_WALL;
      game_map[height - 1][0] = IMMUNE_WALL;
      game_map[height - 1][width - 1] = IMMUNE_WALL;

      // initialize position
      game_map[height / 2][width / 2] = SNAKE_HEAD;
      game_map[height / 2][width / 2 + 1] = SNAKE_BODY;
      game_map[height / 2][width / 2 + 2] = SNAKE_BODY;
    };

    void print_map() const {
      for(int i = 0; i < height; i++){
        for(int j = 0; j < width; j++){
          std::cout << game_map[i][j];
        }
        std::cout << std::endl;
      }
    }

    int get_width() const {
      return width;
    }

    int get_height() const {
      return height;
    }

    int get_coord(int row, int col) const {
      return game_map[row][col];
    }
};

# endif