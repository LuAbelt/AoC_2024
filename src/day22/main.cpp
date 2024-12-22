#include <algorithm>
#include "lib.h"

using namespace std;

i64 getNext(i64 Number) {
  i64 Intermediate = Number * 64;

  Number ^= Intermediate;
  Number %= 16777216;

  Intermediate = Number / 32;
  Number ^= Intermediate;
  Number %= 16777216;

  Intermediate = Number * 2048;
  Number ^= Intermediate;
  Number %= 16777216;

  return Number;
}

void part1(){
  auto Input = util::fromString<i64>(IO::fromCin());

  i64 Res = 0;
  for (i64 Num : Input) {
    cout << Num << ": ";
    for (st i = 0;i<2000;++i) {
      Num = getNext(Num);
    }

    cout << Num << endl;

    Res += Num;
  }

  cout << Res << endl;
}

map<array<i64,4>, i64> calculateSequenceGains(V<i64> &Costs, V<i64> &Diffs) {
  map<array<i64,4>, i64> Result;
  for (st Idx = 4; Idx<Costs.size(); ++Idx) {
    array<i64,4> Seq = {Diffs[Idx-4], Diffs[Idx-3], Diffs[Idx-2], Diffs[Idx-1]};

    if (Result.contains(Seq)) {
      continue;
    }

    //cout << Seq[0] << " " << Seq[1] << " " << Seq[2] << " " << Seq[3] << ": " << Costs[Idx] << endl;

    Result[Seq] = Costs[Idx];
  }

  return Result;
}

void part2() {
  auto Input = util::fromString<i64>(IO::fromCin());

  map<array<i64,4>, i64> Gains;

  for (i64 Num : Input) {
    set<array<i64,4>> Seen;
    i64 Last = Num%10;
    i64 d1  = 0;
    i64 d2  = 0;
    i64 d3  = 0;
    i64 d4  = 0;

    for (st i = 0;i<2000;++i) {
      Num = getNext(Num);

      i64 Cost = Num % 10;
      i64 Diff = (Num % 10) - Last;
      //cout << Num%10 << " (" << (Num % 10) - Last << ")" << endl;
      Last = Num % 10;

      // Shift all last diffs by 1
      d1 = d2;
      d2 = d3;
      d3 = d4;
      d4 = Diff;

      if (i<3) {
        continue;
      }

      array<i64,4> Seq = {d1,d2,d3,d4};
      if (Seen.contains(Seq)) {
        continue;
      }

      Gains[Seq] += Cost;
      Seen += Seq;
    }
  }

  i64 curMax = 0;
  for (auto &[S, Gain] : Gains) {

    curMax = max(curMax, Gain);
  }

  cout << curMax << endl;
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