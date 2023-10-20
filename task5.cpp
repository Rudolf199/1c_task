#include <iostream>
#include <vector>

enum CellState { UNVISITED, VISITED, WALL };

const int maze_size = 6;

class Player {
 private:
  int visited_count = 0;
  std::vector<std::vector<CellState>> visited =
      std::vector<std::vector<CellState>>(
          maze_size, std::vector<CellState>(maze_size, WALL));
  std::vector<int> dx = {-1, 0, 1, 0};
  std::vector<int> dy = {0, 1, 0, -1};
  int x = 0;
  int y = 0;
  int time = 0;

 public:
  int X() { return x; }
  int Y() { return y; }
  int Time() { return time; }
  void Mark() { visited[x][y] = VISITED; }

 public:
  auto Visited() { return visited; }
  auto CellCount() { return visited_count; }
};

class Game {
 private:
  int x_0 = 0;
  int y_0 = 0;
  int x_1 = 0;
  int y_1 = 0;
  int neigh_time = 0;
  int ninety_time = 0;
  int fire_time = 0;
  int fire_K = 0;
  int space = 0;
  std::vector<std::vector<CellState>> real_maze = {
      {WALL, VISITED, WALL, VISITED, WALL, VISITED},
      {VISITED, WALL, VISITED, WALL, VISITED, WALL},
      {WALL, VISITED, WALL, VISITED, WALL, VISITED},
      {VISITED, WALL, VISITED, WALL, VISITED, WALL},
      {WALL, VISITED, WALL, VISITED, WALL, VISITED},
      {VISITED, WALL, VISITED, WALL, VISITED, WALL}};
  std::vector<std::vector<CellState>> live_maze = {
      {WALL, UNVISITED, WALL, UNVISITED, WALL, UNVISITED},
      {UNVISITED, WALL, UNVISITED, WALL, UNVISITED, WALL},
      {WALL, UNVISITED, WALL, UNVISITED, WALL, UNVISITED},
      {UNVISITED, WALL, UNVISITED, WALL, UNVISITED, WALL},
      {WALL, UNVISITED, WALL, UNVISITED, WALL, UNVISITED},
      {UNVISITED, WALL, UNVISITED, WALL, UNVISITED, WALL}};
  Player minotaurs;
  bool CanMove(int X, int Y) {
    if (X < 0 || X > real_maze.size() || Y < 0 || Y > real_maze[0].size()) {
      return false;
    }
    if (real_maze[X][Y] == WALL) {
      return false;
    }
    return true;
  }

 public:
  Game(int& x_0, int& y_0, int& x_1, int& y_1, int& neigh_time,
       int& ninety_time, int& fire_time, int& fire_K)
      : x_0(x_0),
        y_0(y_0),
        x_1(x_1),
        y_1(y_1),
        neigh_time(neigh_time),
        ninety_time(ninety_time),
        fire_time(fire_time),
        fire_K(fire_K) {}

  void Play() {
    int action = 0;
    while (true) {
      if (minotaurs.CellCount() == space) {
        if (minotaurs.Visited() == real_maze) {
          std::cout << "4"
                    << "за " << minotaurs.Time() << "\n";
          break;
        }
      }
      minotaurs.
    }
  }
};

void InitCoordinates(int& x_0, int& y_0, int& x_1, int& y_1) {
  std::cin >> x_0 >> y_0 >> x_1 >> y_1;
  return;
}
void InitTimes(int& neigh_time, int& ninety_time, int& fire_time, int& fire_K) {
  std::cin >> neigh_time >> ninety_time >> fire_time >> fire_K;
  return;
}

void GameRun() {
  int x_0 = 0, y_0 = 0;
  int x_1 = 0, y_1 = 0;
  InitCoordinates(x_0, y_0, x_1, y_1);
  int neigh_time = 0, ninety_time = 0, fire_time = 0, fire_K = 0;
  InitTimes(neigh_time, ninety_time, fire_time, fire_K);
  Game game(x_0, y_0, x_1, y_1, neigh_time, ninety_time, fire_time, fire_K);
}
int main() { return 0; }