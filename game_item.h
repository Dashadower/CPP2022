# ifndef GAMEITEM
# define GAMEITEM

struct GameItem {
  int x;
  int y;
  int remain_time;
  GameItem(int _x, int _y, int _remain_time) : x(_x), y(_y), remain_time(_remain_time) {};
};

# endif
