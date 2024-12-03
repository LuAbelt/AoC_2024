#include <algorithm>
#include "lib.h"

using namespace std;

std::optional<i64> parseNum(string& s, st &pos) {
  i64 res = 0;
  if (isdigit(s[pos])) {
    res *= 10;
    res += (s[pos]-'0');
    ++pos;
  } else {
    return std::nullopt;
  }
  for (int i=0;i<2;++i) {
    if (isdigit(s[pos])) {
      res *= 10;
      res += (s[pos]-'0');
      ++pos;
    } else {
      return res;
    }
  }

  return res;

}

i64 parseMul(string &s, st &pos) {
  if (s[pos]!='m') {
    return 0;
  }
  ++pos;

  if (s[pos]!='u') {
    return 0;
  }
  ++pos;

  if (s[pos]!='l') {
    return 0;
  }
  ++pos;

  if (s[pos]!='(') {
    return 0;
  }
  ++pos;

  auto num1 = parseNum(s,pos);

  if (!num1.has_value()) {
    return 0;
  }

  if (s[pos]!=',') {
    return 0;
  }
  ++pos;

  auto num2 = parseNum(s,pos);

  if (!num2.has_value()) {
    return 0;
  }

  if (s[pos]!=')') {
    return 0;
  }

  return num1.value() * num2.value();
}

std::optional<i32> parseDoDont(string &s, st &pos) {
  if (s[pos] != 'd') {
    return nullopt;
  }
  ++pos;

  if (s[pos] != 'o') {
    return nullopt;
  }
  ++pos;

  switch (s[pos]) {
  case 'n':
    ++pos;
    if (s[pos]!='\'') {
      return nullopt;
    }
    ++pos;

    if (s[pos]!='t') {
      return nullopt;
    }
    ++pos;

    if (s[pos] != '(') {
      return nullopt;
    }
    ++pos;

    if (s[pos] != ')') {
      return nullopt;
    }
    ++pos;

    return 0;
    break;
  case '(':
    ++pos;
    if (s[pos]!=')') {
      return nullopt;
    } else {
      return 1;
    }
    break;
  default:
    return nullopt;
  }
}

void part1(){
  i64 res = 0;
  for (string line; getline(cin, line);) {
    st pos = 0;

    while (pos<line.size()) {
      if (line[pos]!='m') {
        ++pos;
        continue;
      }

      res += parseMul(line, pos);
    }
  }

  cout << res << endl;
}

void part2(){
  i64 res = 0;
  i64 enabled = 1;
  for (string line; getline(cin, line);) {
    st pos = 0;

    while (pos<line.size()) {
      switch (line[pos]) {
      case 'm':
        res += parseMul(line,pos) * enabled;
        break;
      case 'd':
        enabled = parseDoDont(line,pos).value_or(enabled);
        break;
      default:
        ++pos;
      }
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