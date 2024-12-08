#include <algorithm>
#include "lib.h"

using namespace std;
auto parseInput() {
    V<pair<i64, V<i64>>> Equations;

    for (string line;getline(cin, line);) {
        auto Parts = util::split(line, ":");

        i64 Result = util::fromString<i64>({Parts[0]})[0];

        V<i64> Terms = util::fromString<i64>(util::split(Parts[1]));

        Equations += make_pair(Result, Terms);
    }

    return Equations;
}
i64 evaluate(const V<i64> &Terms, V<bool> &Operands) {
    i64 Res = Terms[0];
    auto CurNum = std::next(Terms.begin());

    for (auto b : Operands) {
        if (b) {
            Res *= *CurNum;
        } else {
            Res += *CurNum;
        }
        ++CurNum;
    }

    return Res;
}

V<bool> NumToBoolVec(i64 Num, st Size) {
    V<bool> Res(Size);
    for (st Pos = 0; Pos < Size; ++Pos) {
        Res[Pos] = bits::is_bit_set(Num, Pos);
    }

    return Res;
}

void part1(){
    auto Equations = parseInput();

    i64 Sum = 0;

    for (const auto& [Result, Terms] : Equations) {
        // Brute force all possible combinations
        i64 NumOperands = Terms.size() - 1;
        for (i64 Permutation = 0; Permutation < (1 << NumOperands); ++Permutation) {
            auto Operands = NumToBoolVec(Permutation,NumOperands);

            i64 res = evaluate(Terms,Operands);

            if (res==Result) {
                Sum += Result;
                break;
            }
        }
    }

    cout << Sum << endl;
}

enum class Operand {
    ADD = 0,
    MUL = 1,
    CAT = 2
};

i64 evaluate(const V<i64> &Terms, const V<Operand> &Operands) {
    i64 Res = Terms[0];
    auto CurNum = std::next(Terms.begin());
    auto Operand = Operands.begin();

    while (*Operand == Operand::CAT && Operand != Operands.end()) {
        st NumWidth = trunc(log10(*CurNum))+1;

        Res *= pow(10,NumWidth);
        Res += *CurNum;

        ++Operand, ++CurNum;
    }

    while (Operand!=Operands.end()) {
        auto Op = *Operand;
        i64 NextNum = *CurNum;
        ++Operand;

        while (*Operand == Operand::CAT && Operand != Operands.end()) {
            ++CurNum;
            st NumWidth = trunc(log10(*CurNum))+1;

            NextNum *= pow(10,NumWidth);
            NextNum += *CurNum;
            ++Operand;
        }

        switch (Op) {
            case Operand::ADD:
                Res += NextNum;
                break;
            case Operand::MUL:
                Res *= NextNum;
                break;
            default:
                throw runtime_error("Operator should not occur here!");
        }

        ++CurNum;
    }

    return Res;
}

bool next_combination(V<Operand> &Operands) {
    bool Carry = true;
    for (auto& Op : Operands) {
        switch (Op) {
            case Operand::ADD:
                if (Carry) {
                    Op = Operand::MUL;
                    Carry = false;
                }
                break;
            case Operand::MUL:
                if (Carry) {
                    Op = Operand::CAT;
                    Carry = false;
                }
                break;
            case Operand::CAT:
                if (Carry) {
                    Op = Operand::ADD;
                    Carry = true;
                }
                break;
        }
        if (!Carry) {
            break;
        }
    }

    return !Carry;
}

void part2(){
    auto Equations = parseInput();

    i64 Sum = 0;

    for (const auto& [Result, Terms] : Equations) {
        // Brute force all combinations
        V<Operand> Operands(Terms.size()-1);

        do {
            auto Evaluated = evaluate(Terms, Operands);
            if (Evaluated == Result) {
                Sum += Result;
                break;
            }
        } while (next_combination(Operands));
    }

    cout << Sum << endl;
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