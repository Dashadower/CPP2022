# ifndef GAMESTATE
# define GAMESTATE

#include <iostream>
#include "constants.h"
#include "game_item.h"
#include "game_levels.h"
#include <vector>
#include <deque>
#include <time.h>
#include <sstream>
#include <algorithm>

class GameState{
  /*
  GameState 클래스는 게임 상태를 저장하고 있습니다. 현재 뱀이 어디에 있는지, 아이템들은 어디에
  있는지 등을 2차원 배열을 통해 표현합니다.
  이와 더불어 뱀을 이동시키거나 꼬리를 추가할 때, 뱀의 현재 위치를 vector을 통해 표현하여
  손쉽게 구현이 가능합니다.
  */
  private:
    int width, height; // 게임 맵의 가로,세로 길이
    int **game_map; // game map 2차원 배열 [y][x]
    int direction;  // 뱀의 현재 진행 방향

    std::deque<std::pair<int, int>> snake; // 뱀의 현재 위치 0번째 값이 머리입니다.

    int remain_time_to_generate_growth_item = rand() % 40 + 10;
    std::vector<GameItem> growth_items;

    int remain_time_to_generate_poison_item = rand() % 40 + 10;
    std::vector<GameItem> poison_items;
    // pair는 <y축, x축> index를 담고 있습니다

    std::pair<int, int> gate_1 = std::make_pair(-1,-1);
    std::pair<int, int> gate_2 = std::make_pair(-1,-1);
    int time_until_new_gate = 20;
    
    int max_snake_length = 0;
    int growth_items_used = 0;
    int poison_items_used = 0;
    int gates_used = 0;
    int game_duration_ticks = 0;
    
    std::stringstream debug_ss;  // 디버깅용 cout stream

    Level *current_level = nullptr;

  public:
    GameState(int width, int height, Level* l = nullptr) : width(width), height(height), current_level(l) {
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

      srand(time(NULL));
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


      // load level
      if(current_level != nullptr){
        for(int x = 1; x < width; x++){
          for(int y = 1; y < height; y++){
            game_map[y][x] = current_level->game_map[y][x];
          }
        }
      }
    }

    void load_level(Level* level){
      current_level = level;
      reset_map();
      snake.clear();
      snake.push_back(std::make_pair(height / 2, width / 2));
      snake.push_back(std::make_pair(height / 2, width / 2 + 1));
      snake.push_back(std::make_pair(height / 2, width / 2 + 2));
      direction = LEFT;

      growth_items.clear();
      poison_items.clear();

      gate_1.first = -1;
      gate_1.second = -1;
      gate_2.first = -1;
      gate_2.second = -1;

      max_snake_length = 0;
      growth_items_used = 0;
      poison_items_used = 0;
      gates_used = 0;
      game_duration_ticks = 0;
    }

    bool tick() {
      game_duration_ticks++;
      // 1. Update state
      // 1-1. update snake position
      std::pair<int, int> head = snake.front();
      switch (direction)
      {
      case UP:
        snake.insert(snake.begin(), std::make_pair(head.first, head.second - 1));
        break;

      case DOWN:
        snake.insert(snake.begin(), std::make_pair(head.first, head.second + 1));
        break;

      case LEFT:
        snake.insert(snake.begin(), std::make_pair(head.first - 1, head.second));
        break;

      case RIGHT:
        snake.insert(snake.begin(), std::make_pair(head.first + 1, head.second));
        break;
      }
      snake.pop_back(); // 마지막 값 제거

      // 1-2. Create items
      remain_time_to_generate_growth_item--;
      if (remain_time_to_generate_growth_item == 0) {
        if (growth_items.size() < 3) {

          int x = 0, y = 0;
          while (game_map[y][x] != EMPTY) {
            x = rand() % (width - 2) + 1;
            y = rand() % (height - 2) + 1;
          }

          growth_items.emplace_back(y, x, 50);
        }

        remain_time_to_generate_growth_item = rand() % 40 + 10;
      }

      remain_time_to_generate_poison_item--;
      if (remain_time_to_generate_poison_item == 0) {
        if (poison_items.size() < 3) {

          int x = 0, y = 0;
          while (game_map[y][x] != EMPTY) {
            x = rand() % (width - 2) + 1;
            y = rand() % (height - 2) + 1;
          }

          poison_items.emplace_back(y, x, 50);
        }

        remain_time_to_generate_poison_item = rand() % 40 + 10;
      }

      time_until_new_gate--;
      if (time_until_new_gate == 0){
        int x = rand() % (width - 2) + 1, y = rand() % (height - 2) + 1;
        int direction = rand() % 4;
        while (game_map[y][x] != WALL) {
          switch (direction)
          {
          case 0:
            x++;
            break;
          case 1:
            y++;
            break;
          case 2:
            x--;
            break;
          case 3:
            y--;
            break;
          }
        }
        gate_1.first = x;
        gate_1.second = y;

        x = rand() % (width - 2) + 1, y = rand() % (height - 2) + 1;
        direction = rand() % 4;
        while (game_map[y][x] != WALL) {
          switch (direction)
          {
          case 0:
            x++;
            break;
          
          case 1:
            y++;
            break;
          case 2:
            x--;
            break;
          case 3:
            y--;
            break;
          }
        }
        gate_2.first = x;
        gate_2.second = y;

        time_until_new_gate = rand() % 50 + 10;
      }

      //////////////////////////
      // 2. Perform checks on updated state

      if (game_map[snake[0].second][snake[0].first] == IMMUNE_WALL) {
        return false;
      }

      if (game_map[snake[0].second][snake[0].first] == WALL) {
        debug_ss << '(' << snake[0].first << "," << snake[0].second << ")\n";
        return false;
      }

      for(auto it = ++snake.begin(); it != snake.end(); it++){
        if (it->first == snake.begin()->first && it->second == snake.begin()->second){
              return false;
        }
      }

      for (auto i = 0; i < growth_items.size(); i++) {
        if (
          snake[0].first == growth_items[i].x
          && snake[0].second == growth_items[i].y
        ) {
          growth_items_used++;
          std::pair<int, int> new_head;
          new_head.first = snake[0].first;
          new_head.second = snake[0].second;
          switch (direction) {
             case UP:
              new_head.second--;
              break;

            case DOWN:
              new_head.second++;
              break;

            case LEFT:
              new_head.first--;
              break;

            case RIGHT:
              new_head.first++;
              break;
          }
          if(new_head.first <= 0 || new_head.first >= width || new_head.second <= 0 || new_head.second > height){
            return false;
          }
          snake.push_front(new_head);
          max_snake_length = std::max(max_snake_length, get_current_snake_length());

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
          poison_items_used++;
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

      // gate logic
      for(auto it = snake.begin(); it != snake.end(); it++){
        std::pair<int, int> *exit_gate;
        if(it->first == gate_1.first && it->second == gate_1.second){
          exit_gate = &gate_2;
        }
        else if(it->first == gate_2.first && it->second == gate_2.second){
          exit_gate = &gate_1;
        }
        else continue;
        if(it == snake.begin()) gates_used++;
        time_until_new_gate = snake.end() - it + 5;
        if(exit_gate->first == 0){
          // 출구 게이트가 좌측 벽
          direction = RIGHT;
          it->first = exit_gate->first + 1;
          it->second = exit_gate->second;
        }
        else if(exit_gate->first == width - 1){
          // 출구 게이트가 우측 벽
          direction = LEFT;
          it->first = exit_gate->first - 1;
          it->second = exit_gate->second;
        }
        else if(exit_gate->second == 0){
          // 출구 게이트가 상단 벽
          direction = DOWN;
          it->first = exit_gate->first;
          it->second = exit_gate->second + 1;
        }
        else if(exit_gate->second == height - 1){
          // 출구 게이트가 우측 벽
          direction = UP;
          it->first = exit_gate->first;
          it->second = exit_gate->second - 1;
        }
        else{
          // 맵의 중앙에 있는 벽
          switch (direction)
          {
          case UP:
            if(game_map[exit_gate->second - 1][exit_gate->first] != WALL){
              // 진입 방향과 같은 방향으로 진출
              it->first = exit_gate->first;
              it->second = exit_gate->second - 1;
            }
            else if(game_map[exit_gate->second][exit_gate->first + 1] != WALL){
              // 진입 방향의 시계방향으로 회전 후 진출
              direction = RIGHT;
              it->first = exit_gate->first + 1;
              it->second = exit_gate->second;
            }
            else if(game_map[exit_gate->second][exit_gate->first - 1] != WALL){
              // 진입 방향의 역시계방향으로 회전 후 진출
              direction = LEFT;
              it->first = exit_gate->first - 1;
              it->second = exit_gate->second;
            }
            else{
              // 진입 방향의 반대 방향으로 진출
              direction = DOWN;
              it->first = exit_gate->first;
              it->second = exit_gate->second + 1;
            }
            break;

          case DOWN:
            if(game_map[exit_gate->second + 1][exit_gate->first] != WALL){
              // 진입 방향과 같은 방향으로 진출
              it->first = exit_gate->first;
              it->second = exit_gate->second + 1;
            }
            else if(game_map[exit_gate->second][exit_gate->first - 1] != WALL){
              // 진입 방향의 시계방향으로 회전 후 진출
              direction = LEFT;
              it->first = exit_gate->first - 1;
              it->second = exit_gate->second;
            }
            else if(game_map[exit_gate->second][exit_gate->first + 1] != WALL){
              // 진입 방향의 역시계방향으로 회전 후 진출
              direction = RIGHT;
              it->first = exit_gate->first + 1;
              it->second = exit_gate->second;
            }
            else{
              // 진입 방향의 반대 방향으로 진출
              direction = UP;
              it->first = exit_gate->first;
              it->second = exit_gate->second - 1;
            }
            break;

          case LEFT:
            if(game_map[exit_gate->second][exit_gate->first - 1] != WALL){
              // 진입 방향과 같은 방향으로 진출
              it->first = exit_gate->first - 1;
              it->second = exit_gate->second;
            }
            else if(game_map[exit_gate->second - 1][exit_gate->first] != WALL){
              // 진입 방향의 시계방향으로 회전 후 진출
              direction = UP;
              it->first = exit_gate->first;
              it->second = exit_gate->second - 1;
            }
            else if(game_map[exit_gate->second][exit_gate->first + 1] != WALL){
              // 진입 방향의 역시계방향으로 회전 후 진출
              direction = DOWN;
              it->first = exit_gate->first;
              it->second = exit_gate->second + 1;
            }
            else{
              // 진입 방향의 반대 방향으로 진출
              direction = RIGHT;
              it->first = exit_gate->first + 1;
              it->second = exit_gate->second;
            }
            break;

          case RIGHT:
            if(game_map[exit_gate->second][exit_gate->first + 1] != WALL){
              // 진입 방향과 같은 방향으로 진출
              it->first = exit_gate->first + 1;
              it->second = exit_gate->second;
            }
            else if(game_map[exit_gate->second + 1][exit_gate->first] != WALL){
              // 진입 방향의 시계방향으로 회전 후 진출
              direction = DOWN;
              it->first = exit_gate->first;
              it->second = exit_gate->second + 1;
            }
            else if(game_map[exit_gate->second - 1][exit_gate->first] != WALL){
              // 진입 방향의 역시계방향으로 회전 후 진출
              direction = UP;
              it->first = exit_gate->first;
              it->second = exit_gate->second - 1;
            }
            else{
              // 진입 방향의 반대 방향으로 진출
              direction = LEFT;
              it->first = exit_gate->first - 1;
              it->second = exit_gate->second;
            }
            break;
          }
        }
        break;
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
        game_map[growth_item.y][growth_item.x] = GROWTH_ITEM;
      }

      for (auto it = poison_items.begin(); it != poison_items.end(); it++) {
        auto poison_item = *it;
        game_map[poison_item.y][poison_item.x] = POISON_ITEM;
      }

      // draw gate
      if(gate_1.first != -1){
        game_map[gate_1.second][gate_1.first] = GATE;
      }

      if(gate_2.first != -1){
        game_map[gate_2.second][gate_2.first] = GATE;
      }

      // 현재 뱀의 위치를 순서대로 나타내는 snake가 있을때, snake[0]이 머리가 되고,
      // snake[1] 부터 마지막 값은 몸통이 됩니다. iterator를 통해 snake의 값을 읽고
      // 게임 상태배열에 기록
      std::deque<std::pair<int, int>>::iterator it = snake.begin();
      game_map[it->second][it->first] = SNAKE_HEAD;
      it++;
      while(it != snake.end()) {
        game_map[it->second][it->first] = SNAKE_BODY;
        it++;
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
    void print_debug_info(){
      std::cout << debug_ss.rdbuf();
    }

    int get_width() const {
      // width는 private 변수이기에 함수로 노출시킵니다.
      return width;
    }

    int get_height() const {
      // height private 변수이기에 함수로 노출시킵니다.
      return height;
    }

    int get_current_snake_length() const {
      return snake.size();
    }

    int get_max_snake_length() const {
      return max_snake_length;
    }

    int get_growth_items_used() const {
      return growth_items_used;
    }

    int get_poison_items_used() const {
      return poison_items_used;
    }

    int get_gates_used() const {
      return gates_used;
    }

    int get_b() const {
      //return get_max_snake_length() == 0 ? 0 : get_current_snake_length() / get_max_snake_length();
      return get_current_snake_length();
    }

    int get_goal_b() const {
      if (current_level == nullptr) return 0;
      return current_level->goal_b;
    }

    int get_goal_growth() const {
      if (current_level == nullptr) return 0;
      return current_level->goal_growth;
    }

    int get_goal_poison() const {
      if (current_level == nullptr) return 0;
      return current_level->goal_poison;
    }

    int get_goal_gate() const {
      if (current_level == nullptr) return 0;
      return current_level->goal_gate;
    }

    int get_current_level() const {
      return current_level->level_number;
    }

    int get_game_duration_ticks() const {
      return game_duration_ticks;
    }

    bool check_goal_complete() const {
      if(get_b() >= get_goal_b() &&
         get_growth_items_used() >= get_goal_growth() &&
         get_poison_items_used() >= get_goal_poison() &&
         get_gates_used() >= get_goal_gate()) return true;
      return false;
    }

    int get_coord(int row, int col) const {
      // game_mape 2차원 배열을 private로 선언하였기에 직접 인덱싱을 하지 않고 함수를 통해
      // 값을 설정하게끔 만들어 줍니다.
      return game_map[row][col];
    }
};

# endif
