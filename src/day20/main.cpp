#include <algorithm>
#include "lib.h"

using namespace std;
using transform::Coord2;

auto parseInput() {
  map<Coord2,bool> Walls;

  i64 maxX = 0;
  i64 maxY = 0;
  Coord2 StartPos, EndPos;

  for (string line; getline(cin,line);++maxY) {
    i64 curX = 0;

    for (char c : line) {
      switch (c) {
      case '#':
        Walls[Coord2{curX,maxY}] = true;
        break;
      case 'S':
        StartPos = Coord2{curX,maxY};
        break;
      case 'E':
        EndPos = Coord2{curX,maxY};
      }
      ++curX;
    }


    maxX = max(maxX,curX);
  }

  return make_tuple(Walls, StartPos, EndPos, Coord2{maxX,maxY});
}

V<Coord2> Dirs = {Coord2{1,0},Coord2{0,1},Coord2{-1,0},Coord2{0,-1}};

void part1() {
  auto [Walls, Start, End, GridSize] = parseInput();

  map<Coord2, util::DefaultI64<maxval<i64>()>> Cost;

  using Entry = pair<i64, Coord2>;
  priority_queue<Entry, V<Entry>, greater<Entry>> Open;

  Open += make_pair(0,Start);
  Cost[Start] = 0;

  while (!Open.empty()) {
    auto [PCost, Pos] = Open.top();
    Open.pop();

    if (PCost > Cost[Pos]) {
      continue;
    }

    Cost[Pos] = PCost;

    if (Pos == End) {
      break;
    }

    for (auto &Dir : Dirs) {
      if (Walls[Pos+Dir]) {
        continue;
      }

      Open += make_pair(PCost+1,Pos+Dir);
    }
  }

  cout << Cost[End] << endl;

  i64 Res = 0;
  map<i64,i64> Cheats;

  for (auto &[Pos, PCost] : Cost) {
    if (PCost == maxval<i64>()) {
      continue;
    }

    for (auto& Dir : Dirs) {
      if (!Walls[Pos+Dir]) {
        continue;
      }

      auto CheatPos = Pos + Dir*2;
      if (Walls[CheatPos]) {
        continue;
      }

      if (Cost[CheatPos] == maxval<i64>()) {
        continue;
      }

      i64 Saved = Cost[CheatPos]- PCost - 2;

      if (Saved<0) {
        continue;
      }

      Cheats[Saved]++;

      if ((Saved)>=100) {
        Res++;
      }
    }
  }

  cout << Res << endl;
}

void part2() {
  auto [Walls, Start, End, GridSize] = parseInput();

  map<Coord2, util::DefaultI64<maxval<i64>()>> Cost;

  using Entry = pair<i64, Coord2>;
  priority_queue<Entry, V<Entry>, greater<Entry>> Open;

  Open += make_pair(0,Start);
  Cost[Start] = 0;

  while (!Open.empty()) {
    auto [PCost, Pos] = Open.top();
    Open.pop();

    if (PCost > Cost[Pos]) {
      continue;
    }

    Cost[Pos] = PCost;

    if (Pos == End) {
      break;
    }

    for (auto &Dir : Dirs) {
      if (Walls[Pos+Dir]) {
        continue;
      }

      Open += make_pair(PCost+1,Pos+Dir);
    }
  }

  cout << Cost[End] << endl;

  i64 Res = 0;
  map<i64,i64> Cheats;

  for (const auto &[Pos, PCost] : Cost) {
    if (PCost == maxval<i64>()) {
      continue;
    }

    if (ranges::none_of(Dirs, [&Pos = Pos, &Walls = Walls](auto Dir) {
      return Walls[Pos+Dir];
    })) {
      // Not sure if this can happen, but ignore positions that have no walls beside them
      continue;
    }

    for (i64 xOff = -20; xOff<=20; ++xOff) {
      for (i64 yOff = -20; yOff<=20; ++yOff) {
        Coord2 EndPos = Pos + Coord2{xOff,yOff};

        if ((EndPos-Pos).manhattanLength()>20) {
          continue;
        }

        if (Walls[EndPos]) {
          continue;
        }

        if (Cost[EndPos] == maxval<i64>()) {
          continue;
        }

        i64 Saved = Cost[EndPos] - PCost - (EndPos-Pos).manhattanLength();

        if (Saved <= 0 ) {
          continue;
        }

        Cheats[Saved]++;

        if (Saved >= 100) {
          Res++;
        }
      }
    }
  }

  for (const auto&[Saved, Count]:Cheats) {
    //cout << "There are " << Count << " cheats that save " << Saved << " picoseconds." << endl;
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