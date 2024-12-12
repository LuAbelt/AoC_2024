#include <algorithm>
#include "lib.h"

using namespace std;
using Coord2 = transform::Coord2;

struct DefaultValue {
  char val = '.';

  operator char() const {
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

      Map[Coord2(curX, maxY)] = DefaultValue{c};
      ++curX;
    }
    ++maxY;
  }

  return make_tuple(Map, Coord2(maxX,maxY));
}

auto calculateAreaCircumference(const Coord2 &StartPos, map<Coord2, DefaultValue> &Map, map<Coord2, bool> &Visited) {
  auto Dirs = {Coord2(0,1),Coord2(-1,0),Coord2(0,-1),Coord2(1,0)};

  queue<Coord2> Open;
  Open.push(StartPos);

  i64 Area = 0;
  i64 Circumference = 0;

  while (!Open.empty()) {
    Coord2 CurrentPos = Open.front();
    Open.pop();

    if (Visited[CurrentPos]) {
      continue;
    }

    Visited[CurrentPos] = true;

    Area += 1;
    Circumference += 4;

    for (auto &Dir : Dirs) {
      if (Map[CurrentPos+Dir] != Map[CurrentPos]) {
        continue;
      }

      Circumference--;

      Open.push(CurrentPos+Dir);
    }
  }

  return make_tuple(Area,Circumference);
}

auto calculateAreaSides(const Coord2 &StartPos, map<Coord2, DefaultValue> &Map, map<Coord2, bool> &Visited) {
  array<Coord2, 4> Dirs = {Coord2(0,1),Coord2(-1,0),Coord2(0,-1),Coord2(1,0)};

  queue<Coord2> Open;
  Open.push(StartPos);

  i64 Area = 0;
  i64 Sides = 0;

  map<Coord2, array<bool,4>> Counted;

  while (!Open.empty()) {
    Coord2 CurrentPos = Open.front();
    Open.pop();

    if (Visited[CurrentPos]) {
      continue;
    }

    Visited[CurrentPos] = true;

    Area += 1;

    for (st SideIdx = 0; SideIdx<4;++SideIdx) {
      auto OwnSymbol = Map[CurrentPos];
      if (Map[CurrentPos+Dirs[SideIdx]] == OwnSymbol) {
        continue;
      }

      // We would have a fence in this direction
      // Already mark as counted
      Counted[CurrentPos][SideIdx] = true;

      // Check whether this has already been counted by any of th next positions
      Coord2 Adj1 = CurrentPos + Dirs[(SideIdx+1)%4];
      Coord2 Adj2 = CurrentPos + Dirs[(SideIdx+3)%4];
      if (Counted[Adj1][SideIdx] && (OwnSymbol == Map[Adj1])) {
        Sides--;
      }

      if ((Counted[Adj2][SideIdx] && (OwnSymbol == Map[Adj2]))) {
        Sides--;
      }

      Sides++;
    }

    for (auto &Dir : Dirs) {
      if (Map[CurrentPos+Dir] != Map[CurrentPos]) {
        continue;
      }

      Open.push(CurrentPos+Dir);
    }
  }

  return make_tuple(Area,Sides);
}

void part1(){
  auto [Map, GridSize] = parseInput();

  map<Coord2, bool> Visited;
  map<char, pair<i64,i64>> Results;

  i64 Res = 0;

  for (auto &[Position, Symbol] : Map) {
    if (Visited[Position] || Symbol == '.') {
      continue;
    }

    auto &Entry = Results[Symbol];

    auto [Area,Circ] = calculateAreaCircumference(Position,Map,Visited);

    Entry.first += Area;
    Entry.second += Circ;
    Res += Area*Circ;
  }

  cout << Res << endl;
}

void part2(){
  auto [Map, GridSize] = parseInput();

  map<Coord2, bool> Visited;

  i64 Res = 0;

  for (auto &[Position, Symbol] : Map) {
    if (Visited[Position] || Symbol == '.') {
      continue;
    }

    auto [Area,Sides] = calculateAreaSides(Position,Map,Visited);
    Res += Area*Sides;
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