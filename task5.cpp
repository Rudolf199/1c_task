#include <algorithm>
#include <iostream>
#include <vector>

enum CellState { UNVISITED, VISITED, WALL };
enum Decision { FORWARD, TURN, FIRE };
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
  int dir_code = 0;
  int neigh_time = 0;
  int ninety_time = 0;
  int fire_time = 0;
  int fire_K = 0;
  std::vector<std::vector<CellState>> GetLiveMap() {
    // получаем карту
    return;
  }
  std::pair<int, int> RealCoords(
      const std::vector<std::vector<CellState>>& map) {
    // здесь считаем координаты на мини-карте
    std::pair<int, int> res;
    return res;
  }
  int AnalizeMap() {
    // идем в том направлении где наибольшая прямая.
    std::cout << "3\n";  // запрашиваем радар
    auto map = GetLiveMap();
    int direction = 0;
    std::vector<int> counter(4, -1);
    for (int code = 0; code < 4; ++code) {
      if (code == dir_code) {
        continue;
      }
      counter[code] = 0;
      int step = 1;
      while (map[RealCoords(map).first + step * dx[code]]
                [RealCoords(map).second + step * dy[code]] != WALL) {
        counter[code]++;
        step++;
      }
    }
    direction = std::distance(counter.begin(),
                              std::max_element(counter.begin(), counter.end()));
    return direction;
  }

 public:
  void MakeDecision(const std::vector<std::vector<CellState>>& Map,
                    int step_code) {
    time += neigh_time;
    if (step_code == 1) {
      x += dx[dir_code];
      y += dy[dir_code];
      visited_count++;
      std::cout << "1\n";
      visited[x][y] = VISITED;
      return;
    }
    std::cout << "0\n";  // уперлись в стену, надо искать путь
    time += fire_time;  // разжигаем костер и смотрим куда идти
    auto old_code = dir_code;
    dir_code = AnalizeMap();
    std::cout << "2, " << static_cast<int>(old_code < dir_code)
              << "\n";  // сделали поворот в лучшем направлении
    time += ninety_time;
  }
  Player(int& x_0, int& y_0, int& x_1, int& y_1, int& neigh_time,
         int& ninety_time, int& fire_time, int& fire_K)
      : x(x_0),
        y(y_0),
        neigh_time(neigh_time),
        ninety_time(ninety_time),
        fire_time(fire_time),
        fire_K(fire_K) {
    for (int code = 0; code < 4; code++) {
      if (x_1 - x_0 == dx[code] && y_1 - y_0 == dy[code]) {
        dir_code = code;
      }
    }
  }
  int X() { return x; }
  int Y() { return y; }
  int DirX() { return x + dx[dir_code]; }
  int DirY() { return y + dx[dir_code]; }
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
      {VISITED, WALL, VISITED, WALL, VISITED,
       WALL}};  // ну пусть система автоматически сравнивает со своим
                // лабиринтом(чисто для примера)
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
  std::vector<std::vector<CellState>> SendLiveMap() {
    std::vector<std::vector<CellState>> maze;
    std::string line;
    for (auto row_count = 0; row_count < 2 * fire_K + 1; row_count++) {
      std::cin >> line;  // здесь система "выводит радар"
      std::vector<CellState> row;
      for (const auto c : line) {
        if (c == '_') {
          row.push_back(UNVISITED);
        } else if (c == '#') {
          row.push_back(WALL);
        }
      }
      maze.push_back(row);
    }
    return maze;
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
        fire_K(fire_K),
        minotaurs(x_0, y_0, x_1, y_1, neigh_time, ninety_time, fire_time,
                  fire_K) {}

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
      minotaurs.MakeDecision(SendLiveMap(),
                             CanMove(minotaurs.DirX(), minotaurs.DirY()));
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