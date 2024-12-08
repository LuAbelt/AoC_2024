#include <algorithm>
#include "lib.h"

using namespace std;
using transform::Coord2;

auto parseInput() {
    i64 maxY = 0;
    i64 maxX = 0;

    map<char,V<Coord2>> Antennas;

    for (string line;getline(cin,line);) {
        maxX = line.length();
        i64 curX = 0;
        for (auto c : line ) {
            if (c!='.') {
                Antennas[c] += Coord2(curX,maxY);
            }
            ++curX;
        }
        ++maxY;
    }

    return make_tuple(Antennas, Coord2(maxX,maxY));
}

auto getPointsOnGrid(Coord2 First, Coord2 Second, const Coord2 &GridDims) {
    set<Coord2> Result;

    auto Diff = Second-First;

    i64 Scaling = modulus_calc::greatest_common_divisor(Diff.x(),Diff.y());

    Diff.x() /= Scaling;
    Diff.y() /= Scaling;

    // First search in backwards direction
    Coord2 TestPos = First;

    while (TestPos.x()>=0 && TestPos.y()>=0 && TestPos.x()<GridDims.x() && TestPos.y()<GridDims.y()) {
        auto Dist1 = (First-TestPos).manhattanLength();
        auto Dist2 = (Second-TestPos).manhattanLength();

        if ((Dist2 == 2*Dist1)||(Dist1==2*Dist2)) {
            Result += TestPos;
        }

        TestPos -= Diff;
    }

    TestPos = First;

    // Forward search
    while (TestPos.x()>=0 && TestPos.y()>=0 && TestPos.x()<GridDims.x() && TestPos.y()<GridDims.y()) {
        auto Dist1 = (First-TestPos).manhattanLength();
        auto Dist2 = (Second-TestPos).manhattanLength();

        if ((Dist2 == 2*Dist1)||(Dist1==2*Dist2)) {
            Result += TestPos;
        }

        TestPos += Diff;
    }

    return Result;
}

auto getPointsOnGrid2(Coord2 First, Coord2 Second, const Coord2 &GridDims) {
    set<Coord2> Result;

    auto Diff = Second-First;

    i64 Scaling = modulus_calc::greatest_common_divisor(Diff.x(),Diff.y());

    Diff.x() /= Scaling;
    Diff.y() /= Scaling;

    // First search in backwards direction
    Coord2 TestPos = First;

    while (TestPos.x()>=0 && TestPos.y()>=0 && TestPos.x()<GridDims.x() && TestPos.y()<GridDims.y()) {
        auto Dist1 = (First-TestPos).manhattanLength();
        auto Dist2 = (Second-TestPos).manhattanLength();

        Result += TestPos;

        TestPos -= Diff;
    }

    TestPos = First;

    // Forward search
    while (TestPos.x()>=0 && TestPos.y()>=0 && TestPos.x()<GridDims.x() && TestPos.y()<GridDims.y()) {
        auto Dist1 = (First-TestPos).manhattanLength();
        auto Dist2 = (Second-TestPos).manhattanLength();

        Result += TestPos;

        TestPos += Diff;
    }

    return Result;
}

void part1(){
    auto [Antennas, GridSize] = parseInput();
    set<Coord2> Result;

    for (auto &[_, AntennaCoords] : Antennas) {
        for (const auto &C1 : AntennaCoords) {
            for (const auto &C2 : AntennaCoords) {
                if (C2<=C1) {
                    // We ignore cases in which the coordinates are the same
                    // Additionally, we only want to check each pair once
                    // With this, we always ensure that C1 is "smaller" (In terms of x coordinate) than C2
                    continue;
                }

                auto NewPoints = getPointsOnGrid(C1,C2,GridSize);
                Result.insert(NewPoints.begin(), NewPoints.end());
            }
        }
    }

    cout << Result.size() << endl;
}

void part2(){
    auto [Antennas, GridSize] = parseInput();
    set<Coord2> Result;

    for (auto &[_, AntennaCoords] : Antennas) {
        for (const auto &C1 : AntennaCoords) {
            for (const auto &C2 : AntennaCoords) {
                if (C2<=C1) {
                    // We ignore cases in which the coordinates are the same
                    // Additionally, we only want to check each pair once
                    // With this, we always ensure that C1 is "smaller" (In terms of x coordinate) than C2
                    continue;
                }

                auto NewPoints = getPointsOnGrid2(C1,C2,GridSize);
                Result.insert(NewPoints.begin(), NewPoints.end());
            }
        }
    }

    cout << Result.size() << endl;
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