#include "lib.h"

using namespace std;
struct Gate {
  virtual ~Gate() = default;
  virtual bool evaluate() = 0;
};

map<string, std::unique_ptr<Gate>> Producers;

struct True : public Gate {
  bool evaluate() override {
    return true;
  };
};

struct False : public Gate {
  bool evaluate() override {
    return false;
  }
};

struct Logical : public Gate {
  enum class Type {
    OR,
    AND,
    XOR
  } GType;

  string LHS;
  string RHS;

  Logical(Type T, string lhs, string rhs) : GType(T), LHS(lhs), RHS(rhs){};

  bool evaluate() override {
    if (!Value.has_value()) {
      switch (GType) {
      case Type::OR:
        Value = Producers[LHS]->evaluate() || Producers[RHS]->evaluate();
        break;
      case Type::XOR:
        Value = Producers[LHS]->evaluate() ^ Producers[RHS]->evaluate();
        break;
      case Type::AND:
        Value = Producers[LHS]->evaluate() && Producers[RHS]->evaluate();
        break;
      }
    }

    return Value.value();
  }
private:
  optional<bool> Value;
};

auto parseInputs() {
  for (string Line;getline(cin,Line);) {
    if (Line.empty()) {
      break;
    }

    string NodeName = Line.substr(0,3);

    if (Line[Line.size()-1]=='0') {
      Producers[NodeName] = make_unique<False>();
    } else {
      Producers[NodeName] = make_unique<True>();
    }
  }

  set<string> OutRegisters;

  for (string Line;getline(cin,Line);) {
    auto Parts = util::split(Line, "->");

    string NodeName = Parts[1];

    if (NodeName.starts_with("z")) {
      OutRegisters += NodeName;
    }

    string SplitBy;
    Logical::Type LType;
    if (Parts[0].contains("AND")) {
      SplitBy = "AND";
      LType = Logical::Type::AND;
    }

    if (Parts[0].contains("OR")) {
      SplitBy = "OR";
      LType = Logical::Type::OR;
    }

    if (Parts[0].contains("XOR")) {
      SplitBy = "XOR";
      LType = Logical::Type::XOR;
    }

    auto Operators  = util::split(Parts[0], SplitBy);

    Producers[NodeName] = make_unique<Logical>(LType, Operators[0], Operators[1]);
  }

  return OutRegisters;
}

void part1() {
  auto OutRegisters  = parseInputs();

  i64 curPos = 0;
  i64 Res = 0;
  for (auto& R : OutRegisters) {
    Res += (i64(Producers[R]->evaluate()) << curPos);
    ++curPos;
  }

  cout << Res << endl;
}

void part2() {
  auto OutRegisters = parseInputs();

  map<string, set<string>> Participating;
  for (auto &[Name, Gate] : Producers) {
    if (auto LGate = dynamic_cast<Logical*>(Gate.get())) {
      Participating[LGate->LHS] += Name;
      Participating[LGate->RHS] += Name;
    }
  }

  set<string> WrongRegisters;

  // First check that all z Gates are wired correctly:
  for (i64 Bit = 0; Bit<45;++Bit) {
    string RegName = "z";
    RegName += ((Bit<10)?"0":"") + to_string(Bit);

    if (auto LGate = dynamic_cast<Logical*>(Producers[RegName].get())) {
      if (LGate->GType == Logical::Type::XOR) {
        continue;
      }

      WrongRegisters += RegName;
    } else {
      WrongRegisters += RegName;
    }
  }

  // Try to verify correct result bit
  for (i64 Bit = 1; Bit<45;++Bit) {
    string XRegName = "x";
    XRegName += ((Bit<10)?"0":"") + to_string(Bit);
    string YRegName = XRegName;
    YRegName[0] = 'y';
    string ZRegName = XRegName;
    ZRegName[0] = 'z';

    // Identify first XOR Gate
    string XORReg;
    for (auto &Name : Participating[XRegName]) {
      if (auto LGate = dynamic_cast<Logical*>(Producers[Name].get())) {
        if (LGate->GType != Logical::Type::XOR) {
          continue;
        }

        if (LGate->RHS != YRegName && LGate->LHS != YRegName) {
          continue;
        }

        XORReg = Name;
        break;
      }
    }

    if (auto LGate = dynamic_cast<Logical*>(Producers[ZRegName].get())) {
      if (LGate->RHS == XORReg || LGate->LHS == XORReg) {
        continue;
      }

      WrongRegisters += ZRegName;
    }
  }

  // Try to verify the carry registers
  V<string> CarryRegisters(45,"");
  for (auto Gate : Participating["x00"]) {
    if (auto LGate = dynamic_cast<Logical*>(Producers[Gate].get())) {
      if (LGate->GType != Logical::Type::AND) {
        continue;
      }

      CarryRegisters[0] = Gate;
    }
  }

  for (i64 Bit = 1; Bit<45;++Bit) {
    string XRegName = "x";
    XRegName += ((Bit<10)?"0":"") + to_string(Bit);
    string YRegName = XRegName;
    YRegName[0] = 'y';

    string LastCarry = CarryRegisters[Bit-1];
  }

  for (auto &Name : WrongRegisters) {
    cout << Name << ",";
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