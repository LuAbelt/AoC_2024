#include <algorithm>
#include "lib.h"

using namespace std;

auto parseInputs() {
  V<array<i64, 5>> Keys;
  V<array<i64, 5>> Locks;
  for (string Line; getline(cin,Line);) {
    if (Line.empty()) {
      continue;
    }

    bool isLock = Line.starts_with("#");
    array<i64, 5> Schematic{0,0,0,0,0};

    for (st Pos = 0; Pos < 5; ++Pos) {
      if (Line[Pos] == '#') {
        Schematic[Pos]++;
      }
    }

    for (st i = 0 ; i<6;++i) {
      getline(cin, Line);

      for (st Pos = 0; Pos < 5; ++Pos) {
        if (Line[Pos] == '#') {
          Schematic[Pos]++;
        }
      }
    }

    if (isLock) {
      Locks += Schematic;
    } else {
      Keys += Schematic;
    }
  }

  return make_tuple(Keys, Locks);
}

void part1() {
  auto [Keys, Locks] = parseInputs();

  i64 Res = 0;
  for (auto &Lock : Locks) {
    for (auto &Key : Keys) {
      bool Fit = true;
      for (st Pos = 0; Pos<5; ++Pos) {
        if (Lock[Pos] + Key[Pos] > 7) {
          Fit = false;
          break;
        }
      }

      if (Fit) {
        ++Res;
      }
    }
  }

  cout << Res << endl;
}

void part2() {
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