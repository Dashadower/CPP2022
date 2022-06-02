# ifndef RENDER
# define RENDER

#include <iostream>
#include <ncurses.h>

#include "game_state.h"
#include "constants.h"

class Render{
  /*
  Render 클래스는 게임 상태가 주어졌을때 해당 게임 상태를 화면에 알맞은 형태로 출력하는 함수들을
  담고 있음. 매 tick마다 draw_map()함수를 호출하여 새로 게임 맵을 출력하게끔 코딩해야 함.
  */

  public:
    Render(GameState *gamestate, int map_scale) : gs(gamestate), map_scale(map_scale) {
      // 생성자 인자는 두개이며, 첫번째 인자는 GameState클래스 포인터, 두번째 인자는
      // 게임 맵의 화면 크기를 나타내는 짝수 정수. 예를 들어 맵의 길이가 10x10인 맵을
      // 출력할 때, map_scale이 2인 경우 화면에서 10x10의 크기로, map_scale이 4인 경우
      // 20x20의 크기로 출력됨.
      if(map_scale % 2 != 0){
        throw std::invalid_argument("map scale must be even!");
      }
      initscr();
      curs_set(0);  // hide cursor
      start_color(); // initialize curses colors
      keypad(stdscr, TRUE); // allow special key detection

      int map_width = gs->get_width() * map_scale, map_height = gs->get_height() * (map_scale / 2);
      // map_width, map_height는 게임 맵의 가로, 세로 길이를 의미함.

      map_window = newwin(map_height, map_width, 1, 1);
      // 게임 화면을 나타내는 curses window 포인터 (클래스 변수)
      refresh();
      // ncurses 화면 초기화

      // ncurses에서 사용할 생 정의
      // (정수 ID, 글자 색, 배경 생)
      init_pair(IMMUNE_WALL + 1, COLOR_BLUE, COLOR_BLUE);
      init_pair(WALL + 1, COLOR_BLACK, COLOR_BLACK);
      init_pair(EMPTY + 1, COLOR_WHITE, COLOR_WHITE);
      init_pair(SNAKE_BODY + 1, COLOR_YELLOW, COLOR_YELLOW);
      init_pair(SNAKE_HEAD + 1, COLOR_BLACK, COLOR_BLACK);

      wbkgd(stdscr, COLOR_PAIR(EMPTY + 1));
      // window의 배경 색 설정 (흰색)
      refresh();

    };
    ~Render(){
      endwin();
      // 프로그램 종료 시 ncurses 종료 요청
    }

    void draw_map() {
      // GameState의 map 2차원 배열을 읽어와서 배열의 값에 따라 알맞는 글자를 출력
      // 예) 1(WALL)은 벽을 나타내므로, gs->get_coord(y, x)를 했을때 값이 WALL이면 흰색 문자
      // 를 출력

      for(int y = 0; y < gs->get_height(); y++){
        for(int x = 0; x < gs->get_width(); x++){
          for(int x_scalefactor = 0; x_scalefactor < map_scale; x_scalefactor++){
            for(int y_scalefactor = 0; y_scalefactor < map_scale/2; y_scalefactor++){
              wmove(map_window, y * (map_scale/2) + y_scalefactor, x * map_scale + x_scalefactor);

              int map_value = gs->get_coord(y, x);
              // 현재 좌표에서의 2차원 배열의 값

              switch (map_value)
              {
              case IMMUNE_WALL:
                // 모서리(immune wall)
                waddch(map_window, ' ' | COLOR_PAIR(IMMUNE_WALL + 1));
                break;

              case WALL:
                // 벽(wall)
                waddch(map_window, ' ' | COLOR_PAIR(WALL + 1));
                break;

              case EMPTY:
                // 빈 칸(empty)
                waddch(map_window, ' ' | COLOR_PAIR(EMPTY + 1));
                break;

              case SNAKE_BODY:
                // 뱀 몸통(snake body)
                waddch(map_window, ' ' | COLOR_PAIR(SNAKE_BODY + 1));
                break;

              case SNAKE_HEAD:
                // 뱀 머리(snake head)
                waddch(map_window, ' ' | COLOR_PAIR(SNAKE_HEAD + 1));
                break;
              }

              // ncurses에서 무언가 출력을 했을때는 꼭 refresh()를 이용하여 실제로 터미널에
              // 출력되게끔 해야함. wrefresh(map_window)는 특정 window만 갱신하게끔 만드는
              // 함수임
              wrefresh(map_window);
            }
          }
        }
      }
    };

  private:
    GameState *gs; // 게임 상태를 가지고 있는 클래스
    WINDOW *map_window; // 게임 맵을 출력하는 ncurses window 포인터
    int map_scale; // 게임 맵의 크기를 설정하는 설정값
};

# endif
