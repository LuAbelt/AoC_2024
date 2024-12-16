#include <algorithm>
#include "lib.h"

using namespace std;
using Coord2 = transform::Coord2;
struct ClawMachine {
  Coord2 AButton;
  Coord2 BButton;
  Coord2 PriceLocation;
};

auto parseInput() {
  V<ClawMachine> Result;

  std::regex ButtonRegx(".*X\\+(\\d+),.*Y\\+(\\d+)");
  std::regex PrizeRegx(".*X=(\\d+),.*Y=(\\d+)");

  for (string line1;getline(cin,line1);) {
    if (line1.empty()) {
      continue;
    }
    string line2,line3;
    getline(cin,line2);
    getline(cin,line3);

    std::smatch matches;

    regex_search(line1, matches, ButtonRegx);

    i64 x,y;
    stringstream ss;

    ss = stringstream{matches[1].str()};
    ss >> x;

    ss = stringstream{matches[2].str()};
    ss >> y;
    Coord2 AButton{x,y};

    regex_search(line2, matches, ButtonRegx);
    ss = stringstream{matches[1].str()};
    ss >> x;

    ss = stringstream{matches[2].str()};
    ss >> y;
    Coord2 BButton{x,y};


    regex_search(line3, matches, PrizeRegx);
    ss = stringstream{matches[1].str()};
    ss >> x;

    ss = stringstream{matches[2].str()};
    ss >> y;
    Coord2 Target{x,y};

    Result += ClawMachine{AButton, BButton, Target};

  }
  return Result;
}

void ctos(Coord2 C) {
  cout << "(" << C.x() << "|" << C.y() << ")";
}

i64 getMinCost(ClawMachine &Machine) {
  i64 Min = maxval<i64>();

  Coord2 Remaining = Machine.PriceLocation;
  i64 CurrentCost = 0;

  while (Remaining.x() > 0
    && Remaining.y() > 0) {
    if ((Remaining.x() % Machine.BButton.x()) == 0
       && (Machine.BButton.y() * (Remaining.x()/Machine.BButton.x())) == Remaining.y()) {
      Min = min(Min, CurrentCost + (Remaining.x()/Machine.BButton.x()));
    }

    Remaining -= Machine.AButton;
    CurrentCost += 3;
  }

  return Min == maxval<i64>()?0:Min;
}

i64 getMinCostFast(ClawMachine &Machine) {
  auto &AButton = Machine.AButton;
  auto &BButton = Machine.BButton;
  auto &PriceLoc = Machine.PriceLocation;
  // Case 1: Check if A/B are linearly dependent
  if (
    AButton.x() * BButton.y() == AButton.y() * BButton.x()
  ) {
    i64 ACost = maxval<i64>();
    i64 BCost = maxval<i64>();

    if (PriceLoc.x() % AButton.x() == 0
      && PriceLoc.y() == AButton.y() * (PriceLoc.x()/AButton.x())) {
      ACost = 3 *PriceLoc.x()/AButton.x();
    }

    if (PriceLoc.x() % BButton.x() == 0
      && PriceLoc.y() == BButton.y() * (PriceLoc.x()/BButton.x())) {
      BCost = PriceLoc.x()/BButton.x();
    }

    if (ACost != maxval<i64>() || BCost != maxval<i64>()) {
      return min(ACost,BCost);
    }

    return 0;
  }

  // Other case: Solve linear equation system
  i64 xa = AButton.x();
  i64 ya = AButton.y();
  i64 xb = BButton.x();
  i64 yb = BButton.y();
  i64 xp = PriceLoc.x();
  i64 yp = PriceLoc.y();

  i64 det = (xa*yb) - (ya*xb);
  i64 det1 = (xp*yb) - (yp*xb);
  i64 det2 = (xa*yp) - (ya*xp);

  i64 a = det1/det;
  i64 b = det2/det;

  if ((AButton*a) + (BButton*b) == PriceLoc) {
    return 3*a+b;
  }
  return 0;
}



void part1(){
  auto Machines = parseInput();

  i64 Res = 0;
   for (auto& Machine : Machines) {
     Res += getMinCost(Machine);
   }

  cout << Res << endl;
}

void part2(){
  auto Machines = parseInput();

  for (auto &Machine : Machines) {
    Machine.PriceLocation += Coord2{10000000000000,10000000000000};
  }

  i64 Res = 0;
  for (auto& Machine : Machines) {
    i64 Cost = getMinCostFast(Machine);
    Res += Cost;
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