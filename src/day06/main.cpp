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

void part1(){
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

void part2(){
  auto [MaxPos, Blocked, CurrentPos] = parseInput();

  Coord2 Dirs[4] = {Coord2(0,-1), Coord2(1,0),Coord2(0,1),Coord2(-1,0)};
  st currentDir = 0;

  map<st, V<Coord2>> BlockedByX;
  map<st, V<Coord2>> BlockedByY;

  for (const auto [coord, _] : Blocked) {
    BlockedByX[coord.x()] += coord;
    BlockedByY[coord.y()] += coord;
  }

  set<array<Coord2,4>> Candidates;

  for (const auto&[CurrentCoord, _] : Blocked) {
    if (CurrentCoord.x() == 0) {
      continue;
    }
    // Checks cases in which lower left/right corner is missing
    for (const Coord2 &UpperRight : BlockedByY[CurrentCoord.y()+1]) {
      const Coord2 &UpperLeft = CurrentCoord;
      if (UpperRight.x() <= UpperLeft.x()) {
        continue;
      }

      // Check if a potential lower right exists
      for (const Coord2 &LowerRight : BlockedByX[UpperRight.x()-1]) {
        if (LowerRight.y() <= UpperRight.y()) {
          continue;
        }

        if (UpperLeft.x() == 0) {
          continue;
        }

        // Calculate lower left corner position
        Coord2 LowerLeft(UpperLeft.x()-1,LowerRight.y()-1);

        // Check if coord is already blocked and/or starting position
        if (LowerLeft == CurrentPos || Blocked[LowerLeft]) {
          continue;
        }

        Candidates += array<Coord2,4>{UpperLeft, UpperRight, LowerRight, LowerLeft};
      }

      // Same but for lower left
      for (const Coord2 &LowerLeft : BlockedByX[UpperLeft.x()-1]) {
        if (LowerLeft.y() <= UpperLeft.y()) {
          continue;
        }

        // Calculate lower right corner position
        Coord2 LowerRight(UpperRight.x()-1,LowerLeft.y()+1);

        Candidates += array<Coord2,4>{UpperLeft, UpperRight, LowerRight, LowerLeft};

        // Check if coord is already blocked and/or starting position
        if (LowerRight == CurrentPos || Blocked[LowerRight]) {
          continue;
        }

        Candidates += array<Coord2,4>{UpperLeft, UpperRight, LowerRight, LowerLeft};
      }
    }

    // Check cases in which upper right is missing, by fixing lower left
    for (const Coord2 &LowerLeft : BlockedByX[CurrentCoord.x()-1]) {
      const Coord2 &UpperLeft = CurrentCoord;
      if (LowerLeft.y() <= UpperLeft.y()) {
        continue;
      }

      for (const Coord2 &LowerRight : BlockedByY[LowerLeft.y()+1]) {
        if (LowerRight.x() <= LowerLeft.x()) {
          continue;
        }

        Coord2 UpperRight(LowerRight.x()+1,UpperLeft.y()+1);

        if ( UpperRight.x()>=MaxPos.x() || UpperRight == CurrentPos || Blocked[UpperRight]) {
          continue;
        }

        Candidates += array<Coord2,4>{UpperLeft, UpperRight, LowerRight, LowerLeft};
      }
    }

    // Cases in which the upper left corner is missing
    const Coord2 &UpperRight = CurrentCoord;
    for (const Coord2 &LowerRight : BlockedByX[UpperRight.x()-1]) {
      if (LowerRight.y()<=UpperRight.y()) {
        continue;
      }

      for (const Coord2 &LowerLeft : BlockedByY[LowerRight.y()-1]) {
        if (LowerLeft.x()>=LowerRight.x()) {
          continue;
        }

        //Calculate Upper Left Corner position
        Coord2 UpperLeft(LowerLeft.x()+1,UpperRight.y()-1);

        if (UpperLeft == CurrentPos || Blocked[UpperLeft]) {
          continue;
        }

        Candidates += array<Coord2, 4>{UpperLeft, UpperRight, LowerRight, LowerLeft};
      }
    }
  }

  map<pair<Coord2,st>, bool> Visited;
  while (CurrentPos.x()>=0 && CurrentPos.x() < MaxPos.x() && CurrentPos.y()>= 0 && CurrentPos.y()<MaxPos.y()) {
    while (Blocked[CurrentPos+Dirs[currentDir]]) {
      Visited[make_pair(CurrentPos,currentDir)] = true;
      currentDir = (currentDir + 1)%4;
    }

    Visited[make_pair(CurrentPos,currentDir)] = true;

    CurrentPos += Dirs[currentDir];
  }

  i64 res = 0;

  for (auto &Candidate : Candidates) {
    for (st Direction = 0; Direction < 4; ++Direction) {
      if (!Visited[make_pair(Candidate[Direction]-Dirs[Direction], Direction)]) {
        continue;
      }

      // Additional Validity checking
      cout << "Candidate test: ";
      for (st Direction = 0; Direction < 4; ++Direction) {
        cout << "(" << Candidate[Direction].x() << "|" << Candidate[Direction].y() << ") ";
      }
      cout << endl;
      // Check that all edges are actually traversable
      Coord2 UpperLeft(Candidate[0].x(),Candidate[1].y());
      Coord2 LowerRight(Candidate[2].x(),Candidate[3].y());
      // Left Border
      bool traversable = ranges::all_of(
          BlockedByX[UpperLeft.x()], [&UpperLeft,LowerRight](Coord2 C) {
        return C.y() < UpperLeft.y() || C.y() > LowerRight.y();
      });

      // Right Border
      traversable &= ranges::all_of(
        BlockedByX[LowerRight.x()], [&UpperLeft,&LowerRight](Coord2 C) {
          return C.y() < UpperLeft.y() || C.y() > LowerRight.y();
        }
      );

      // Upper border
      traversable &= ranges::all_of(
        BlockedByY[UpperLeft.y()], [&UpperLeft,&LowerRight](Coord2 C) {
          return C.x() < UpperLeft.x() || C.x() > LowerRight.x();
        }
      );

      // Lower border
      traversable &= ranges::all_of(
        BlockedByY[LowerRight.y()], [&UpperLeft,&LowerRight](Coord2 C) {
          return C.x() < UpperLeft.x() || C.x() > LowerRight.x();
        }
      );
      if (!traversable){
        break;
      }

      cout << "Candidate viable: ";
      for (st Direction = 0; Direction < 4; ++Direction) {
        cout << "(" << Candidate[Direction].x() << "|" << Candidate[Direction].y() << ") ";
      }
      cout << endl;
      ++res;
      break;
    }
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