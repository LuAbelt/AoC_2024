#include <algorithm>
#include "lib.h"

using namespace std;

auto parseInput() {


  string input;
  getline(cin,input);

  V<string> Patterns = util::split(input, ",");

  // Skip empty line
  getline(cin,input);
  V<string> Designs = IO::fromCin();

  return make_tuple(Patterns, Designs);
}

string buildRegEx(V<string> &Strings) {
  string Result = "(";

  for (auto& s : Strings) {
    Result += s;
    Result += "|";
  }

  Result[Result.size()-1] = ')';
  Result += "*";

  return Result;
}

void part1(){
  auto [Patterns, Designs] = parseInput();

  string PatternRegex = buildRegEx(Patterns);
  regex re(PatternRegex);
  i64 Res = 0;

  for (auto &Design : Designs) {
    std::smatch base_match;
    if (regex_match(Design, base_match, re)) {
      ++Res;
    }
  }

  cout << Res << endl;
}

V<map<i64,i64>> preprocessString(const string &S,const V<string> &Patterns) {
  V<map<i64,i64>> Result(S.size());

  for (st Pos = 0; Pos<S.size();++Pos) {
    for (const auto &Pattern : Patterns) {
      if (Pattern == S.substr(Pos, Pattern.size())) {
        Result[Pos][Pattern.size()]++;
      }
    }
  }

  return Result;
}

void part2(){
  auto [Patterns, Designs] = parseInput();

  string PatternRegex = buildRegEx(Patterns);
  regex re(PatternRegex);
  i64 Res = 0;

  for (auto &Design : Designs) {
    std::smatch base_match;
    if (!regex_match(Design, base_match, re)) {
      continue;
    }

    auto PatternMap = preprocessString(Design, Patterns);

    V<i64> DP(Design.size()+1);
    DP[0] = 1;

    for (st Pos = 0;Pos<Design.size();++Pos) {
      for (auto &[Length, Count]: PatternMap[Pos]) {
        DP[Pos+Length] += DP[Pos]*Count;
      }
    }

    Res += DP[Design.size()];
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