#include <iostream>

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
        game_map[height - 1][x] = 1;
      }
      for(int y = 0; y < height; y++){
        game_map[y][0] = 1;
        game_map[y][width - 1] = 1;
      }
      game_map[0][0] = 2;
      game_map[0][width - 1] = 2;
      game_map[height - 1][0] = 2;
      game_map[height - 1][width - 1] = 2;
    };

    void print_map(){
      for(int i = 0; i < height; i++){
        for(int j = 0; j < width; j++){
          std::cout << game_map[i][j];
        }
        std::cout << std::endl;
      }
    }

    int get_width(){
      return width;
    }

    int get_height(){
      return height;
    }

    int get_coord(int row, int col){
      return game_map[row][col];
    }
};

# endif