#include <algorithm>
#include "lib.h"

using namespace std;

using Coord2 = transform::Coord<2>;
auto parseInput() {
  st maxX = 0;
  st maxY = 0;
  map<Coord2,bool> Blocked;
  Coord2 StartPos;
  for (string line;getline(cin,line);++maxY) {
    maxX = line.length();

    st curX = 0;
    for (auto c : line) {
      switch (c) {
      case '#':
        Blocked[Coord2(i64(curX), i64(maxY))] = true;
        break;
      case '^':
        StartPos = Coord2(i64(curX), i64(maxY));
        break;
      }
      ++curX;
    }
  }

  return make_tuple(Coord2(i64(maxX),i64(maxY)),Blocked,StartPos);
}

void part1() {
  auto [MaxPos, Blocked, CurrentPos] = parseInput();

  Coord2 Dirs[4] = {Coord2(0,-1), Coord2(1,0),Coord2(0,1),Coord2(-1,0)};
  st currentDir = 0;

  set<Coord2> Visited;

  while (CurrentPos.x()>=0 && CurrentPos.x() < MaxPos.x() && CurrentPos.y()>= 0 && CurrentPos.y()<MaxPos.y()) {
    Visited += CurrentPos;

    while (Blocked[CurrentPos+Dirs[currentDir]]) {
      currentDir = (currentDir + 1)%4;
    }

    CurrentPos += Dirs[currentDir];
  }

  cout << Visited.size() << endl;
}

auto CompareByX = [](const Coord2 &C1, const Coord2 &C2) {
  return C1.x() < C2.x();
};

auto CompareByY = [](const Coord2 &C1, const Coord2 &C2) {
  return C1.y() < C2.y();
};

using CoordSetX = set<Coord2, decltype(CompareByX)>;
using CoordSetY = set<Coord2, decltype(CompareByY)>;

Coord2 Dirs[4] = {Coord2(0,-1), Coord2(1,0),Coord2(0,1),Coord2(-1,0)};

Coord2 getNextPosition(const Coord2 &CurrentPos, st Direction,map<st, CoordSetY> &BlockedByX, map<st, CoordSetX> &BlockedByY) {
  switch (Direction) {
    case 0: {
      // Moving upwards, try to find a bonk with lower y value

      auto Bonk = ranges::lower_bound(BlockedByX[CurrentPos.x()],CurrentPos);

      if (Bonk == BlockedByX[CurrentPos.x()].begin()) {
        // We would exit the map
        return {-1,-1};
      }

      --Bonk;

      return (*Bonk)-Dirs[Direction];
    }
    break;
    case 1: {
      // Moving right, find first with higher x value
      auto Bonk = ranges::lower_bound(BlockedByY[CurrentPos.y()], CurrentPos);

      if (Bonk == BlockedByY[CurrentPos.y()].end()) {
        // We would exit the map
        return {-1,-1};
      }

      return (*Bonk)-Dirs[Direction];
    }
    break;
    case 2:
      // Moving down, find first with higher y value
    {
      auto Bonk = ranges::lower_bound(BlockedByX[CurrentPos.x()],CurrentPos);

      if (Bonk == BlockedByX[CurrentPos.x()].end()) {
        // We would exit the map
        return {-1,-1};
      }

      return (*Bonk)-Dirs[Direction];
    }
    break;
    case 3: {
      // Moving left, find first with lower x value
      auto Bonk = ranges::lower_bound(BlockedByY[CurrentPos.y()], CurrentPos);

      if (Bonk == BlockedByY[CurrentPos.y()].begin()) {
        // We would exit the map
        return {-1,-1};
      }

      --Bonk;

      return (*Bonk)-Dirs[Direction];
    }
    break;
    default:
      throw(runtime_error("Invalid Dir Index!"));
  }
}

bool hasCircles(Coord2 Pos, st DirIdx, map<st, CoordSetY> &BlockedByX, map<st, CoordSetX> &BlockedByY, map<Coord2, bool> &Blocked) {
  // Stores the coordinates where we already bonked and the new direction
  map<pair<Coord2,st>, bool> Bonked;

  while (true) {
    Bonked[make_pair(Pos,DirIdx)] = true;

    Pos = getNextPosition(Pos,DirIdx, BlockedByX, BlockedByY);

    if (Pos == Coord2{-1,-1}) {
      return false;
    }

    while (Blocked[Pos+Dirs[DirIdx]]) {
      DirIdx = (DirIdx+1) % 4;
    }

    if (Bonked[make_pair(Pos,DirIdx)]) {
      return true;
    }
  }
}

ostream& operator<<(ostream & lhs, add_const<transform::Coord<2>>::type & rhs) {
  lhs << "(" << rhs.x() << "|" << rhs.y() << ")";
  return lhs;
};

void testPositions(map<Coord2, bool> &Blocked, map<st, CoordSetY> &BlockedByX, map<st, CoordSetX> &BlockedByY) {
  V<pair<Coord2, st>> StartPositions = {
    {Coord2(0,0), 1},
    {Coord2(0,0), 3},
    {Coord2(4,0), 1},
    {Coord2(4,0), 3},
    {Coord2(8,0), 1},
    {Coord2(8,0), 3},
    {Coord2(4,1), 1},
    {Coord2(4,1), 3},
    {Coord2(8,1), 1},
    {Coord2(8,1), 3},
    {Coord2(9,0),0},
    {Coord2(9,0),2},
    {Coord2(9,4),0},
    {Coord2(9,4),2},
    {Coord2(9,8),0},
    {Coord2(9,8),2},
    {Coord2(8,4),0},
    {Coord2(8,4),2},
    {Coord2(8,8),0},
    {Coord2(8,8),2},

  };

  for (const auto &[StartPos, DirIdx] : StartPositions) {
    auto NextPos = getNextPosition(StartPos,DirIdx,BlockedByX, BlockedByY);
    cout << "StartPos: " << StartPos << ", Direction: " << DirIdx << ". Next Position: " << NextPos << endl;
  }
}

void part2(){
  auto [MaxPos, Blocked, CurrentPos] = parseInput();
  const Coord2 StartPos = CurrentPos;

  st currentDir = 0;

  map<st, CoordSetY> BlockedByX;
  map<st, CoordSetX> BlockedByY;

  for (const auto [coord, _] : Blocked) {
    BlockedByX[coord.x()] += coord;
    BlockedByY[coord.y()] += coord;
  }

  set<Coord2> Candidates;

  map<Coord2, bool> Visited;

  st res = 0;

  while (CurrentPos.x()>=0 && CurrentPos.x() < MaxPos.x() && CurrentPos.y()>= 0 && CurrentPos.y()<MaxPos.y()) {
    while (Blocked[CurrentPos+Dirs[currentDir]]) {
      Visited[CurrentPos] = true;
      currentDir = (currentDir + 1)%4;
    }

    Visited[CurrentPos] = true;

    {
      //Check whether blocking the space in front of us creates a loop
      auto BlockedPos = CurrentPos+Dirs[currentDir];

      if ( BlockedPos.x() >= 0 && BlockedPos.y() >= 0
        && BlockedPos.x() < MaxPos.x() && BlockedPos.y() < MaxPos.y()
        && BlockedPos != StartPos
        && !Visited[BlockedPos]) {
        Blocked[BlockedPos] = true;
        BlockedByY[BlockedPos.y()] += BlockedPos;
        BlockedByX[BlockedPos.x()] += BlockedPos;

        // At least one space should still be free, otherwise we just trapped in the guard
        if (any_of(std::begin(Dirs), std::end(Dirs),[&Blocked = Blocked, &CurrentPos = CurrentPos](auto Dir) {
          return !Blocked[CurrentPos+Dir];
        })) {
          if (hasCircles(CurrentPos, (currentDir+1)%4, BlockedByX, BlockedByY, Blocked)) {
            ++res;
          }
        }

        BlockedByX[BlockedPos.x()].erase(BlockedPos);
        BlockedByY[BlockedPos.y()].erase(BlockedPos);
        Blocked[CurrentPos+Dirs[currentDir]] = false;
      }
    }

    CurrentPos += Dirs[currentDir];
  }

  cout << res << endl;
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