#include <algorithm>
#include "lib.h"

using namespace std;
using transform::Coord2;

V<Coord2> Dirs = {Coord2(1,0),Coord2(0,1),Coord2(-1,0),Coord2(0,-1)};

auto parseInput() {
  map<Coord2, bool> Walls;
  map<Coord2, bool> Boxes;
  Coord2 StartPos;
  V<i64> Moves;

  i64 maxY = 0;
  i64 maxX = 0;
  for (string line; getline(cin,line);++maxY) {
    if (line.empty()) {
      break;
    }

    i64 curX = 0;

    for (char c : line) {
      switch (c) {
      case '#':
        Walls[Coord2{curX,maxY}] = true;
        break;
      case 'O':
        Boxes[Coord2{curX,maxY}] = true;
        break;
      case '@':
        StartPos = Coord2{curX,maxY};
        break;
      default:;
      }

      ++curX;
    }

    maxX = max(maxX,curX);
  }

  for (string moves; getline(cin,moves);){
    for (char c : moves) {
      switch (c) {
      case '>':
        Moves.push_back(0);
        break;
      case 'v':
        Moves.push_back(1);
        break;
      case '<':
        Moves.push_back(2);
        break;
      case '^':
        Moves.push_back(3);
        break;
      }
    }
  }

  return make_tuple(Walls, Boxes, StartPos, Moves, Coord2{maxX,maxY});
}

auto parseInputPart2() {
  map<Coord2, bool> Walls;
  map<Coord2, bool> Boxes;
  Coord2 StartPos;
  V<i64> Moves;

  i64 maxY = 0;
  i64 maxX = 0;
  for (string line; getline(cin,line);++maxY) {
    if (line.empty()) {
      break;
    }

    i64 curX = 0;

    for (char c : line) {
      switch (c) {
      case '#':
        Walls[Coord2{curX,maxY}] = true;
        Walls[Coord2{curX+1,maxY}] = true;
        break;
      case 'O':
        Boxes[Coord2{curX,maxY}] = true;
        break;
      case '@':
        StartPos = Coord2{curX,maxY};
        break;
      default:;
      }

      curX+=2;
    }

    maxX = max(maxX,curX);
  }

  for (string moves; getline(cin,moves);){
    for (char c : moves) {
      switch (c) {
      case '>':
        Moves.push_back(0);
        break;
      case 'v':
        Moves.push_back(1);
        break;
      case '<':
        Moves.push_back(2);
        break;
      case '^':
        Moves.push_back(3);
        break;
      }
    }
  }

  return make_tuple(Walls, Boxes, StartPos, Moves, Coord2{maxX,maxY});
}

bool move(const Coord2& Position,const Coord2 &Direction, map<Coord2,bool> &Walls, map<Coord2, bool> &Boxes) {
  if (Walls[Position]) {
    return false;
  }

  if (!Boxes[Position]) {
    return true;
  }

  if (move(Position+Direction, Direction, Walls, Boxes)) {
    Boxes[Position] = false;
    Boxes[Position+Direction] = true;
    return true;
  }

  return false;
}

bool canMove(const Coord2& Position,i64 DirIdx, map<Coord2,bool> &Walls, map<Coord2, bool> &Boxes) {
  if (Walls[Position]) {
    return false;
  }

  switch (DirIdx) {
  case 0:
    //Right movement, simple check for boxes
    if (!Boxes[Position]) {
      // No box in target position, can move
      return true;
    }
    // Box in target position, check if we can move that box
    // "Skip" 1 position, as this is still the same box
    return canMove(Position+(Dirs[DirIdx]*2), DirIdx, Walls, Boxes);
    break;
  case 2:
    // Left movement, check position 1 to the left, as we only store leftmost position of boxes
    if (!Boxes[Position+Dirs[DirIdx]]) {
      return true;
    }

    // Box in target position, check if we can move the box
    return canMove(Position+(Dirs[DirIdx]*2), DirIdx, Walls, Boxes);
    break;
  case 1: [[fallthrough]];
  case 3:
    // Vertical movement, check both explicit position and position to the left of it,
    if (!(Boxes[Position] || Boxes[Position+Dirs[2]] )) {
      return true;
    }

    Coord2 Pos1 = Position+Dirs[DirIdx];
    Coord2 Pos2 = Position+Dirs[DirIdx]+Dirs[0];
    if (Boxes[Position+Dirs[2]]) {
      Pos2 = Position+Dirs[DirIdx]+Dirs[2];
    }

    return canMove(Pos1, DirIdx, Walls,Boxes) && canMove(Pos2, DirIdx, Walls,Boxes);
    break;
  }

  return false;
}

void movePart2(const Coord2& Position, i64 DirIdx, const map<Coord2, bool> &Walls, map<Coord2, bool> &Boxes) {
  if (!(Boxes[Position] || Boxes[Position+Dirs[2]])){
    return;
  }


  Coord2 Pos1 = Position+Dirs[DirIdx];
  switch (DirIdx) {
  case 0:
    movePart2(Position+Dirs[DirIdx]*2, DirIdx, Walls, Boxes);

    Boxes[Position] = false;
    Boxes[Position+Dirs[DirIdx]] = true;
  break;
  case 2:
    movePart2(Position+Dirs[DirIdx]*2, DirIdx, Walls, Boxes);
    Boxes[Position+Dirs[2]] = false;
    Boxes[Position+Dirs[2]*2] = true;
  break;
  case 1:
  case 3:
    Coord2 Pos2 = Position+Dirs[DirIdx]+Dirs[0];
    if (Boxes[Position+Dirs[2]]) {
      Pos2 = Position+Dirs[DirIdx]+Dirs[2];
    }

    movePart2(Pos1, DirIdx, Walls, Boxes);
    movePart2(Pos2, DirIdx, Walls, Boxes);

    if (Boxes[Position]) {
      Boxes[Position] = false;
      Boxes[Pos1] = true;
    }

    if (Boxes[Position+Dirs[2]]) {
      Boxes[Position+Dirs[2]] = false;
      Boxes[Pos2] = true;
    }
    break;
  }
}

void print(Coord2 &Position, map<Coord2, bool> &Walls, map<Coord2, bool> &Boxes, Coord2 & GridSize) {
  V<string> Output(GridSize.y(),string(GridSize.x(), '.'));

  Output[Position.y()][Position.x()] = '@';

  for (auto& [Pos, val] : Walls) {
    if (!val) {
      continue;
    }

    Output[Pos.y()][Pos.x()] = '#';
  }

  for (auto& [Pos, val] : Boxes) {
    if (!val) {
      continue;
    }

    Output[Pos.y()][Pos.x()] = 'O';
  }

  for (auto& S : Output) {
    cout << S << endl;
  }
  cout << endl << endl;
}

void printPart2(Coord2 &Position, map<Coord2, bool> &Walls, map<Coord2, bool> &Boxes, Coord2 & GridSize) {
  V<string> Output(GridSize.y(),string(GridSize.x(), '.'));

  Output[Position.y()][Position.x()] = '@';

  for (auto& [Pos, val] : Walls) {
    if (!val) {
      continue;
    }

    Output[Pos.y()][Pos.x()] = '#';
  }

  for (auto& [Pos, val] : Boxes) {
    if (!val) {
      continue;
    }

    Output[Pos.y()][Pos.x()] = '[';
    Output[Pos.y()][Pos.x()+1] = ']';
  }

  for (auto& S : Output) {
    cout << S << endl;
  }
  cout << endl << endl;
}

i64 calculateGPS(map<Coord2, bool> &Boxes) {
  i64 Result = 0;

  for (auto &[Coord, val] : Boxes) {
    if (!val) {
      continue;
    }

    Result += 100*Coord.y() + Coord.x();
  }

  return Result;
}

void part1() {
  auto [Walls, Boxes, CurrentPos, Moves, GridSize] = parseInput();

  //cout << Moves.size() << endl;
  //cout << "Initial state: " << endl;
  //print(CurrentPos,Walls,Boxes,GridSize);

  //string S = ">v<^";
  for (i64 Move : Moves) {
    Coord2 &Dir = Dirs[Move];
    //cout << "Move " << S[Move] << ':' << endl;
    if (move(CurrentPos + Dir, Dir, Walls, Boxes)) {
      CurrentPos += Dir;
    }

    //print(CurrentPos,Walls,Boxes,GridSize);
  }
  cout << "Final state: " << endl;
  print(CurrentPos,Walls,Boxes,GridSize);

  cout << calculateGPS(Boxes) << endl;
}

void part2(){
  auto [Walls, Boxes, CurrentPos, Moves, GridSize] = parseInputPart2();

  cout << "Initial State: "<<endl;
  printPart2(CurrentPos,Walls, Boxes, GridSize);

  //string S{">v<^"};
  for (i64 MoveIdx : Moves) {
    //cout << "Move " << S[MoveIdx] << endl;

    if (canMove(CurrentPos+Dirs[MoveIdx], MoveIdx, Walls, Boxes)) {
      movePart2(CurrentPos+Dirs[MoveIdx], MoveIdx, Walls, Boxes);
      CurrentPos += Dirs[MoveIdx];
    }

    //printPart2(CurrentPos,Walls, Boxes, GridSize);
  }

  cout << "Final state: " << endl;
  printPart2(CurrentPos,Walls,Boxes,GridSize);

  cout << calculateGPS(Boxes) << endl;
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