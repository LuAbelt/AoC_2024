#include <algorithm>
#include "lib.h"


using namespace std;
void part1(){
  using Coord = transform::Coord<2>;
  
  i64 res = 0;
  std::map<Coord, char> grid;
  i64 maxX = 0;
  i64 maxY = 0;
  i64 count = 0;
  for (string line; getline(cin, line);) {
    i64 curX = 0;

    for (const char c : line) {
      grid[Coord(curX,maxY)] = c;
      ++curX;
    }

    maxX = max(maxX, i64(line.length()));
    ++maxY;
  }

  auto dirs = { Coord(0,1) ,Coord(1,1), Coord(1,0),
                                 Coord(1,-1), Coord(0,-1),Coord(-1,-1),
                                Coord(-1,0), Coord(-1,1)};

  for (auto &[p,c] : grid) {
    if (c != 'X') {
      continue;
    }

    for (auto& dir : dirs) {
      if (grid[p+dir] == 'M'
          && grid[p+(dir*2)] == 'A'
          && grid[p+(dir*3)] == 'S') {
        ++count;
      }
    }
  }

  cout << count << endl;
}

void part2(){
  using Coord = transform::Coord<2>;

  i64 res = 0;
  std::map<Coord, char> grid;
  i64 maxX = 0;
  i64 maxY = 0;
  i64 count = 0;
  for (string line; getline(cin, line);) {
    i64 curX = 0;

    for (const char c : line) {
      grid[Coord(curX,maxY)] = c;
      ++curX;
    }

    maxX = max(maxX, i64(line.length()));
    ++maxY;
  }

  auto dirs = { Coord(1,1), Coord(1,-1),Coord(-1,-1), Coord(-1,1)};

  for (auto &[p,c] : grid) {
    if (c != 'M') {
      continue;
    }

    for (auto& dir : dirs) {
      if (grid[p+dir] != 'A'
          || grid[p+(dir*2)] != 'S') {
        continue;
      }

      Coord rot{-dir.y(),dir.x()};

      if ((grid[p+dir+rot] == 'M' && grid[p+dir-rot] == 'S')
        ||(grid[p+dir+rot] == 'S' && grid[p+dir-rot] == 'M')) {
        ++count;
      }
    }
  }

  cout << count/2 << endl;
}

int main(int argc, char* argv[]) {
    if(argc!=2)
    {
        std::cout << "Wrong number of arguments." << std::endl;
        return 0;
    }
    if(std::string(argv[1])=="--one"){
        part1();
    }else if(std::string(argv[1])=="--two"){
        part2();
    } else{
        std::cout << "Wrong usage!" << std::endl;
    }
}