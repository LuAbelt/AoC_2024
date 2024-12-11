#include <algorithm>
#include "lib.h"

using namespace std;
auto parseInput() {
  string line;
  getline(cin,line);

  auto Stones = util::fromString<i64>(util::split(line));

  map<ui64,ui64> Result;
  for (auto S : Stones) {
    Result[S]++;
  }

  return Result;
}

map<ui64,ui64> Blink(const map<ui64, ui64> &Stones) {
  map<ui64,ui64> Result;

  for (const auto&[Value, Count] : Stones) {
    if (Value == 0) {
      Result[1] += Count;
      continue;
    }

    auto NumDigits = util::numDigits(Value);

    if (NumDigits % 2 == 0) {
      //TODO
      ui64 NumLeft = Value / (i64(pow(10,NumDigits/2)));
      ui64 NumRight = Value % (i64(pow(10,NumDigits/2)));
      Result[NumLeft] += Count;
      Result[NumRight] += Count;
      continue;
    }

    Result[Value*2024] += Count;
  }

  return Result;
}

void part1(){
  auto Stones = parseInput();

  for (int i = 0; i<25;++i) {
    Stones = Blink(Stones);
  }

  i64 Res = 0;
  for (const auto&[_,Count]: Stones) {
    Res += Count;
  }

  cout << Res << endl;
}

void part2(){
  auto Stones = parseInput();

  for (int i = 0; i<75;++i) {
    Stones = Blink(Stones);
  }

  ui64 Res = 0;
  for (const auto&[_,Count]: Stones) {
    Res += Count;
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