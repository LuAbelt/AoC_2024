#include <algorithm>
#include "lib.h"

using namespace std;

i64 getComboOperand(i64 Operand, i64 RegisterA, i64 RegisterB, i64 RegisterC) {
  switch (Operand) {
  case 0:[[fallthrough]];
  case 1:[[fallthrough]];
  case 2:[[fallthrough]];
  case 3:
    return Operand;
    break;
  case 4:
    return RegisterA;
    break;
  case 5:
    return RegisterB;
    break;
  case 6:
    return RegisterC;
    break;
  default:
    cout << "Invalid Combo Operand: " << Operand << endl;
    return -1;
  }
}

bool runMachine(const V<i64> &Instructions, st &InsPtr, i64 &RegisterA, i64 &RegisterB, i64 &RegisterC, V<i64> &Outputs) {
  if (Instructions.size() <= InsPtr) {
    return false;
  }

  i64 OpCode = Instructions[InsPtr];
  i64 Operand = Instructions[InsPtr+1];

  switch (OpCode) {
  case 0:
    Operand = getComboOperand(Operand, RegisterA, RegisterB, RegisterC);

    Operand = (1<<(Operand>63?63:Operand));
    RegisterA /= Operand;
    break;
  case 1:
    RegisterB = RegisterB ^ Operand;
    break;
  case 2:
    Operand = getComboOperand(Operand,RegisterA,RegisterB,RegisterC);
    RegisterB = Operand % 8;
    break;
  case 3:
    if ( RegisterA != 0 ) {
      InsPtr = Operand;
      return true;
    }
    break;
  case 4:
    RegisterB = RegisterB ^ RegisterC;
    break;
  case 5:
    Operand = getComboOperand(Operand, RegisterA, RegisterB, RegisterC);
    Outputs += Operand%8;
    break;
  case 6:
    Operand = getComboOperand(Operand, RegisterA, RegisterB, RegisterC);

    Operand = (1<<(Operand>63?63:Operand));
    RegisterB = RegisterA / Operand;
    break;
  case 7:
    Operand = getComboOperand(Operand, RegisterA, RegisterB, RegisterC);

    Operand = (1<<(Operand>63?63:Operand));
    RegisterC = RegisterA / Operand;
    break;
  default:
    cout << "Invalid OpCode: " << OpCode << endl;
    return false;
    break;
  }

  InsPtr += 2;
  return true;
}

auto parseInput() {
  string line;
  getline(cin,line);

  i64 RegA = atol(util::split(line)[2].c_str());

  getline(cin,line);
  i64 RegB = atol(util::split(line)[2].c_str());
  getline(cin,line);
  i64 RegC = atol(util::split(line)[2].c_str());
  getline(cin,line);
  getline(cin,line);

  V<i64> Program = util::fromString<i64>(util::split(util::split(line)[1], ","));

  return make_tuple(RegA,RegB,RegC,Program);
}

void part1(){
  auto [RegisterA, RegisterB, RegisterC, Program] = parseInput();

  st InstPtr = 0;
  V<i64> Outputs;

  while (runMachine(Program, InstPtr, RegisterA, RegisterB, RegisterC, Outputs)) {

  }

  for (auto& V : Outputs) {
    cout << V << ",";
  }
}

bool findMatchingPosition(i64& RegABase, V<i64> &Program, V<i64> &Expected,
                          const st Idx) {

  for (st Offset = 0; Offset<8;++Offset) {
    i64 RegA = RegABase + Offset;
    i64 RegB = 0;
    i64 RegC = 0;

    V<i64> Outputs;
    st InstPtr = 0;
    while (runMachine(Program, InstPtr, RegA, RegB, RegC, Outputs)) {

    }

    if (Outputs[0] == Expected[Idx]) {
      i64 NewBase = RegABase + Offset;
      if (Idx == 0) {
        RegABase = NewBase;
        return true;
      }
      NewBase <<= 3;
      if ( findMatchingPosition(NewBase, Program, Expected, Idx-1)) {
        RegABase = NewBase;
        return true;
      }
    }
  }

  return false;
}

void part2(){
  auto [Ra, Rb, Rc, Program] = parseInput();

  V<i64> ExpectedOutput = Program;
  st Idx = ExpectedOutput.size()-1;

  i64 RegisterA = 0;
  if (!findMatchingPosition(RegisterA, Program, ExpectedOutput, Idx)) {
    cout << "Impossible" << endl;
  }

  cout << RegisterA << endl;

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