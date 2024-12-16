#include <algorithm>
#include "lib.h"

using namespace std;
using Coord2 = transform::Coord2;

struct Robot {
  Coord2 StartPos;
  Coord2 Velocity;
};
void ctos(Coord2 C) {
  cout << "(" << C.x() << "|" << C.y() << ")";
}

auto parseInput() {
  V<Robot> Result;
  for (string line;getline(cin,line);) {
    auto parts = util::split(line);

    auto coordS = util::split(parts[0],"=");
    auto Vals = util::fromString<i64>(util::split(coordS[1],","));

    Coord2 Start{Vals[0],Vals[1]};

    coordS = util::split(parts[1],"=");
    Vals = util::fromString<i64>(util::split(coordS[1],","));

    Coord2 Velo{Vals[0],Vals[1]};

    Result += Robot{Start,Velo};
  }

  return Result;
}

void part1() {
  auto Robots = parseInput();

  Coord2 Dimensions{101,103};
  V<Coord2> EndPositions;

  for (auto &Robot: Robots) {
    while (Robot.Velocity.x()<0) {
      Robot.Velocity.x() += Dimensions.x();
    }

    while (Robot.Velocity.y()<0) {
      Robot.Velocity.y() += Dimensions.y();
    }

    Coord2 EndPos = Robot.StartPos + Robot.Velocity*100;
    EndPos.x() %= Dimensions.x();
    EndPos.y() %= Dimensions.y();

    EndPositions += EndPos;
  }

  array<i64,4> Quadrants{0,0,0,0};

  for (auto &EndPos : EndPositions) {
    if (EndPos.x() == (Dimensions.x()/2) || EndPos.y() == (Dimensions.y()/2)) {
      continue;
    }

    //ctos(EndPos); cout << "//";

    auto idx = 0;
    if (EndPos.x() > (Dimensions.x()/2)) {
      idx += 1;
    }

    if (EndPos.y() > (Dimensions.y()/2)) {
      idx +=2;
    }

    //cout << idx << endl;
    Quadrants[idx]++;
  }

  cout << Quadrants[0]*Quadrants[1]*Quadrants[2]*Quadrants[3] << endl;
}

void print(V<Robot> &Robots) {
  V<string> Output(103, string(101,'.'));

  for (auto &Robot : Robots) {
    Output[Robot.StartPos.y()][Robot.StartPos.x()] = '#';
  }

  for (auto &S : Output) {
    cout << S << endl;
  }
}

bool treeLike(V<Robot> &Robots) {
  V<V<bool>> Output(103,V<bool>(101, false));

  for (auto &Robot : Robots) {
    Output[Robot.StartPos.y()][Robot.StartPos.x()] = true;
  }

  auto Dirs  = {Coord2{1,0},Coord2{1,1},Coord2{0,1},
    Coord2{-1,1},Coord2{-1,0},Coord2{-1,-1}
    ,Coord2{0,-1},Coord2{1,-1}};

  i64 Result = 0;

  for (i64 x = 1; x<100; x++) {
    for (i64 y = 1; y<102; ++y) {
      i64 nCnt = 0;

      for (auto& Dir : Dirs) {
        Coord2 Pos{x+Dir.x(),y+Dir.y()};
        if (Output[Pos.y()][Pos.x()]) {
          ++nCnt;
        }
      }

      if (nCnt==8) {
        ++Result;
      }
    }
  }

  return Result > 30;
}

void part2() {
  auto Robots = parseInput();

  for (auto &Robot: Robots) {
    while (Robot.Velocity.x()<0) {
      Robot.Velocity.x() += 101;
    }

    while (Robot.Velocity.y()<0) {
      Robot.Velocity.y() += 103;
    }
  }

  for (st Seconds = 0;Seconds<101*103;++Seconds) {
    if (treeLike(Robots)) {
      cout << "Second " << Seconds << endl;
      print(Robots);
    }

    for (auto& Robot : Robots) {
      Robot.StartPos += Robot.Velocity;

      Robot.StartPos.x() %= 101;
      Robot.StartPos.y() %= 103;
    }
  }
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