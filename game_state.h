#include <iostream>
#include "constants.h"
#include <vector>

# ifndef GAMESTATE
# define GAMESTATE

class GameState{
  private:
    int width, height;
    int **game_map;
    int direction;
    std::vector<std::pair<int, int>> snake; // snake (y, x)

  public:
    GameState(int width, int height) : width(width), height(height) {
      game_map = new int *[height];
      for(int i = 0; i < height; i++){
        game_map[i] = new int[width];
      }
      reset_map();
      snake.push_back(std::make_pair(height / 2, width / 2));
      snake.push_back(std::make_pair(height / 2, width / 2 + 1));
      snake.push_back(std::make_pair(height / 2, width / 2 + 2));
      direction = LEFT;
    };

    void reset_map(){
      // write empty and walls into game_map
      // reset array to 0
      for(int x = 1; x < width; x++){
        for(int y = 1; y < height; y++){
          game_map[y][x] = 0;
        }
      }

      // create walls
      for(int x = 0; x < width; x++){
        game_map[0][x] = 1;
        game_map[height - 1][x] = WALL;
      }
      for(int y = 0; y < height; y++){
        game_map[y][0] = 1;
        game_map[y][width - 1] = WALL;
      }

      // create corners
      game_map[0][0] = IMMUNE_WALL;
      game_map[0][width - 1] = IMMUNE_WALL;
      game_map[height - 1][0] = IMMUNE_WALL;
      game_map[height - 1][width - 1] = IMMUNE_WALL;
    }

    void tick(){
      reset_map();

      // update snake position
      std::pair<int, int> head = snake.front();
      switch (direction)
      {
      case UP:
        snake.insert(snake.begin(), std::make_pair(head.first - 1, head.second));
        break;
      
      case DOWN:
        snake.insert(snake.begin(), std::make_pair(head.first + 1, head.second));
        break;

      case LEFT:
        snake.insert(snake.begin(), std::make_pair(head.first, head.second - 1));
        break;

      case RIGHT:
        snake.insert(snake.begin(), std::make_pair(head.first, head.second + 1));
        break;
      }
      snake.pop_back(); // remove tail

      // actuate new snake position to map
       std::vector<std::pair<int, int>>::iterator it = snake.begin();
       game_map[it->first][it->second] = SNAKE_HEAD;
       it++;
       while(it != snake.end()){
         game_map[it->first][it->second] = SNAKE_BODY;
         it++;
       }
    }

    void set_direction(int d){
      if (direction != d) direction = d;
    }

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