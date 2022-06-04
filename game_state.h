# ifndef GAMESTATE
# define GAMESTATE

#include <iostream>
#include "constants.h"
#include "game_item.h"
#include <vector>
#include <deque>
#include <time.h>

class GameState{
  /*
  GameState 클래스는 게임 상태를 저장하고 있습니다. 현재 뱀이 어디에 있는지, 아이템들은 어디에
  있는지 등을 2차원 배열을 통해 표현합니다.
  이와 더불어 뱀을 이동시키거나 꼬리를 추가할 때, 뱀의 현재 위치를 vector을 통해 표현하여
  손쉽게 구현이 가능합니다.
  */
  private:
    int width, height; // 게임 맵의 가로,세로 길이
    int **game_map; // game map 2차원 배열
    int direction;  // 뱀의 현재 진행 방향

    std::deque<std::pair<int, int>> snake; // 뱀의 현재 위치 0번째 값이 머리입니다.

    int remain_time_to_generate_growth_item = rand() % 40 + 10;
    std::vector<GameItem> growth_items;

    int remain_time_to_generate_poison_item = rand() % 40 + 10;
    std::vector<GameItem> poison_items;
    // pair는 <y축, x축> index를 담고 있습니다.

  public:
    GameState(int width, int height) : width(width), height(height) {
      // 생성자 함수는 두개의 인자를 받으며, 각각 게임 맵의 가로, 세로 길이를 나타냅니다.

      // 게임 맵의 크기가 정해지면, 2차원 배열을 동적으로 생성합니다.
      game_map = new int *[height];
      for(int i = 0; i < height; i++){
        game_map[i] = new int[width];
      }

      // 기본 맵을 세팅합니다.
      reset_map();

      //이후 뱀의 초기 위치 및 크기를 정해줍니다. (맵의 정가운데)
      snake.push_back(std::make_pair(height / 2, width / 2));
      snake.push_back(std::make_pair(height / 2, width / 2 + 1));
      snake.push_back(std::make_pair(height / 2, width / 2 + 2));
      direction = LEFT;
    };

    void reset_map(){
      // 게임을 진행하는 동안 모서리와 벽의 위치는 변하지 않습니다.
      // 이처럼 게임 로직과 관계없이 일정한 요소들을 저장하여 게임 상태를 나타내는 2차원 배열의
      // 바탕이 되게끔 합니다.

      // 모든 타일을 빈칸으로 초기화
      for(int x = 1; x < width; x++){
        for(int y = 1; y < height; y++){
          game_map[y][x] = 0;
        }
      }

      // 테두리를 따라서 벽 생성
      for(int x = 0; x < width; x++){
        game_map[0][x] = 1;
        game_map[height - 1][x] = WALL;
      }
      for(int y = 0; y < height; y++){
        game_map[y][0] = 1;
        game_map[y][width - 1] = WALL;
      }

      // 모서리 4개 생성
      game_map[0][0] = IMMUNE_WALL;
      game_map[0][width - 1] = IMMUNE_WALL;
      game_map[height - 1][0] = IMMUNE_WALL;
      game_map[height - 1][width - 1] = IMMUNE_WALL;
    }

    bool tick() {
      for (auto i = 0; i < growth_items.size(); i++) {
        if (
          snake[0].first == growth_items[i].x
          && snake[0].second == growth_items[i].y
        ) {
          std::pair<int, int> new_head;
          new_head.first = snake[0].first;
          new_head.second = snake[0].second;
          switch (direction) {
             case UP:
              new_head.first--;
              break;

            case DOWN:
              new_head.first++;
              break;

            case LEFT:
              new_head.second--;
              break;

            case RIGHT:
              new_head.second++;
              break;
          }
          snake.push_front(new_head);

          growth_items.erase(growth_items.begin() + i);
          continue;
        }

        growth_items[i].remain_time--;
        if (growth_items[i].remain_time == 0) {
          growth_items.erase(growth_items.begin() + i);
        }
      }

      for (auto i = 0; i < poison_items.size(); i++) {
        if (
          snake[0].first == poison_items[i].x
          && snake[0].second == poison_items[i].y
        ) {
          if (snake.size() == 3) {
            return false;
          }

          snake.pop_back();

          poison_items.erase(poison_items.begin() + i);
          continue;
        }

        poison_items[i].remain_time--;
        if (poison_items[i].remain_time == 0) {
          poison_items.erase(poison_items.begin() + i);
        }
      }

      // 게임은 일정 시간마다 뱀의 위치가 움직이게끔 설정해야 함.
      // tick()함수는 현재 상태에서 뱀의 방향에 따라 한칸 전진하게끔 만들어줌.
      // 구현 방법은:
      // 1. 현재 뱀의 머리 앞에 새로운 칸 추가
      // 2. 뱀의 마지막 칸 삭제
      reset_map();

      // Appear items
      for (auto it = growth_items.begin(); it != growth_items.end(); it++) {
        auto growth_item = *it;
        game_map[growth_item.x][growth_item.y] = GROWTH_ITEM;
      }

      for (auto it = poison_items.begin(); it != poison_items.end(); it++) {
        auto poison_item = *it;
        game_map[poison_item.x][poison_item.y] = POISON_ITEM;
      }

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
      snake.pop_back(); // 마지막 값 제거

      // 현재 뱀의 위치를 순서대로 나타내는 snake가 있을때, snake[0]이 머리가 되고,
      // snake[1] 부터 마지막 값은 몸통이 됩니다. iterator를 통해 snake의 값을 읽고
      // 게임 상태배열에 기록
      std::deque<std::pair<int, int>>::iterator it = snake.begin();
      game_map[it->first][it->second] = SNAKE_HEAD;
      it++;
      while(it != snake.end()) {
        game_map[it->first][it->second] = SNAKE_BODY;
        it++;
      }

      remain_time_to_generate_growth_item--;
      if (remain_time_to_generate_growth_item == 0) {
        if (growth_items.size() < 3) {
          GameItem growth_item;

          int x = rand() % 23 + 1;
          int y = rand() % 23 + 1;
          while (game_map[x][y] != 0) {
            x = rand() % 23 + 1;
            y = rand() % 23 + 1;
          }

          growth_item.x = x;
          growth_item.y = y;
          growth_item.remain_time = 50;

          growth_items.push_back(growth_item);
        }

        remain_time_to_generate_growth_item = rand() % 40 + 10;
      }

      remain_time_to_generate_poison_item--;
      if (remain_time_to_generate_poison_item == 0) {
        if (poison_items.size() < 3) {
          GameItem poison_item;

          int x = rand() % 23 + 1;
          int y = rand() % 23 + 1;
          while (game_map[x][y] != 0) {
            x = rand() % 23 + 1;
            y = rand() % 23 + 1;
          }

          poison_item.x = x;
          poison_item.y = y;
          poison_item.remain_time = 50;

          poison_items.push_back(poison_item);
        }

        remain_time_to_generate_poison_item = rand() % 40 + 10;
      }

      return true;
    }

    void set_direction(int d){
      // 사용자로부터 새로운 방향 d를 입력받았을때, 만약 새로운 방향이 현재 방향의 반대가 아니라면
      // 새로운 방향으로 설정
      if (direction == d) return;
      if (direction % 2 == d % 2) return;
      direction = d;
    }

    void print_map() const {
      // 디버깅용 함수이며, ncurses없이 2차원 게임 상태배열을 출력
      for(int i = 0; i < height; i++){
        for(int j = 0; j < width; j++){
          std::cout << game_map[i][j];
        }
        std::cout << std::endl;
      }
    }

    int get_width() const {
      // width는 private 변수이기에 함수로 노출시킵니다.
      return width;
    }

    int get_height() const {
      // height private 변수이기에 함수로 노출시킵니다.
      return height;
    }

    int get_coord(int row, int col) const {
      // game_mape 2차원 배열을 private로 선언하였기에 직접 인덱싱을 하지 않고 함수를 통해
      // 값을 설정하게끔 만들어 줍니다.
      return game_map[row][col];
    }
};

# endif
