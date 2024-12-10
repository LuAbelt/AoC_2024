#include <algorithm>
#include "lib.h"

using namespace std;
using Coord2 = transform::Coord2;

struct DefaultValue {
  i64 val = -1;

  operator i64() const {
    return val;
  }
};

auto parseInput() {
  map<Coord2, DefaultValue> Map;
  i64 maxY = 0;
  i64 maxX = 0;
  for (string line;getline(cin,line);) {
    i64 curX = 0;
    maxX = max(maxX,i64(line.length()));
    for (auto& c : line) {
      if (!isdigit(c)) {
        ++curX;
        continue;
      }

      Map[Coord2(curX, maxY)] = DefaultValue{c-'0'};
      ++curX;
    }
    ++maxY;
  }

  return make_tuple(Map, Coord2(maxX,maxY));
}

i64 findPaths(const Coord2 Start, map<Coord2, DefaultValue> &Map) {
  map<Coord2, bool> Visited;

  auto Dirs = {Coord2(1,0),Coord2(0,1),Coord2(-1,0),Coord2(0,-1)};

  queue<Coord2> Open;

  Open += Start;
  set<Coord2> Results;

  while (!Open.empty()) {
    Coord2 Current = Open.front();
    Open.pop();

    if (Visited[Current]) {
      continue;
    }
    Visited[Current] = true;

    if (Map[Current] == 9) {
      Results += Current;
      continue;
    }

    for (auto &Dir : Dirs) {
      auto NewPos = Current+Dir;

      if (Map[Current]+1 == Map[NewPos]) {
        Open += NewPos;
      }
    }
  }

  return Results.size();
}


i64 findAllPaths(const Coord2 Start, map<Coord2, DefaultValue> &Map) {
  auto Dirs = {Coord2(1,0),Coord2(0,1),Coord2(-1,0),Coord2(0,-1)};

  queue<Coord2> Open;

  Open += Start;
  i64 Result = 0;

  while (!Open.empty()) {
    Coord2 Current = Open.front();
    Open.pop();

    if (Map[Current] == 9) {
      Result += 1;
      continue;
    }

    for (auto &Dir : Dirs) {
      auto NewPos = Current+Dir;

      if (Map[Current]+1 == Map[NewPos]) {
        Open += NewPos;
      }
    }
  }

  return Result;
}

void part1() {
  auto [Map, GridSize] = parseInput();

  i64 Res = 0;
  for (const auto &[Position, Value] : Map ) {
    if (Value == 0) {
      Res += findPaths(Position,Map);
    }
  }

  cout << Res << endl;
}

void part2(){
  auto [Map, GridSize] = parseInput();

  i64 Res = 0;
  for (const auto &[Position, Value] : Map ) {
    if (Value == 0) {
      Res += findAllPaths(Position,Map);
    }
  }

  cout << Res << endl;
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