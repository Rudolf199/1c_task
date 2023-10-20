#include <algorithm>
#include <iostream>
#include <unordered_map>
#include <vector>

enum CellState { UNVISITED, VISITED, WALL };
enum Decision { FORWARD, TURN, FIRE };
const int maze_size = 6;

namespace std {
template <>
struct hash<std::pair<int, int>> {
  size_t operator()(const std::pair<int, int>& p) const {
    size_t h1 = std::hash<int>{}(p.first);
    size_t h2 = std::hash<int>{}(p.second);
    return h1 ^ (h2 << 1);
  }
};
}  // namespace std

class Player {
 private:
  int visited_count = 0;
  std::vector<int> dx = {-1, 0, 1, 0};
  std::vector<int> dy = {0, 1, 0, -1};
  int x = 0;
  int y = 0;
  std::unordered_map<std::pair<int, int>, CellState> VisitedMap;
  int time = 0;
  int dir_code = 0;
  int neigh_time = 0;
  int ninety_time = 0;
  int fire_time = 0;
  int fire_K = 0;
  std::vector<std::vector<CellState>> GetLiveMap() {
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
  std::pair<int, int> RealCoords(
      const std::vector<std::vector<CellState>>& map) {
    std::pair<int, int> res = {fire_K,
                               fire_K};  // только понял, что там стены будут
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
        if (VisitedMap.find({x + step * dx[code], y + step * dy[code]}) ==
            VisitedMap.end()) {
          counter[code]++;
        }
        step++;
      }
    }
    direction = std::distance(counter.begin(),
                              std::max_element(counter.begin(), counter.end()));
    return direction;
  }

 public:
  void MakeDecision(int step_code) {
    time += neigh_time;
    if (step_code == 1) {
      if (VisitedMap.find({x + dx[dir_code], y + dy[dir_code]}) ==
          VisitedMap.end()) {
        x += dx[dir_code];
        y += dy[dir_code];
        visited_count++;
        std::cout << "1\n";
        // visited[x][y] = VISITED;
        VisitedMap[{x, y}] = VISITED;
        return;
      }
      x += dx[dir_code];
      y += dy[dir_code];
      std::cout << "1\n";
      time += fire_time;
      auto old_code = dir_code;
      dir_code = AnalizeMap();
      if (dir_code != old_code) {
        std::cout << "2, " << static_cast<int>(old_code < dir_code) << "\n";
        time += ninety_time;
      }
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
  bool Finished() {
    auto map = GetLiveMap();
    int wall_or_unvisited_count = 0;
    for (int dir = 0; dir < 4; ++dir) {
      if (map[fire_K + dx[dir]][fire_K + dy[dir]] == WALL) {
        wall_or_unvisited_count++;
      }
      if (VisitedMap.find({x + dx[dir], y + dy[dir]}) != VisitedMap.end()) {
        wall_or_unvisited_count++;
      }
    }
    if (map[fire_K - 1][fire_K - 1] == WALL) {
      wall_or_unvisited_count++;
    }
    if (map[fire_K + 1][fire_K + 1] == WALL) {
      wall_or_unvisited_count++;
    }
    if (map[fire_K - 1][fire_K + 1] == WALL) {
      wall_or_unvisited_count++;
    }
    if (map[fire_K + 1][fire_K - 1] == WALL) {
      wall_or_unvisited_count++;
    }
    if (VisitedMap.find({x + 1, y + 1}) != VisitedMap.end()) {
      wall_or_unvisited_count++;
    }
    if (VisitedMap.find({x + 1, y - 1}) != VisitedMap.end()) {
      wall_or_unvisited_count++;
    }
    if (VisitedMap.find({x - 1, y + 1}) != VisitedMap.end()) {
      wall_or_unvisited_count++;
    }
    if (VisitedMap.find({x - 1, y - 1}) != VisitedMap.end()) {
      wall_or_unvisited_count++;
    }
    return (wall_or_unvisited_count == 8);
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

 public:
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
  Player minotaurs;
  bool CanMove(int X, int Y) {
    int can_move = 0;
    std::cin >> can_move;  // система говорит можем двигаться или нет
    return static_cast<bool>(can_move);
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
      if (minotaurs.Finished()) {
        std::cout << "4"
                  << "за " << minotaurs.Time() << "\n";
        break;
      }
      minotaurs.MakeDecision(CanMove(minotaurs.DirX(), minotaurs.DirY()));
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
  game.Play();
}
int main() {
  GameRun();
  return 0;
}