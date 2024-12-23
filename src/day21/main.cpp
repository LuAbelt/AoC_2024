#include <algorithm>
#include "lib.h"

using namespace std;
using transform::Coord2;

V<string> NumPad  = {
  "789",
  "456",
  "123",
  " 0A"
};

V<string> DirectionalPad = {
  " ^A",
  "<v>"
};

using CharToCoord = map<char, Coord2>;
using CoordToChar = map<Coord2, char>;

pair<CharToCoord, CoordToChar> buildMaps(V<string> Mapping) {
  CharToCoord Result1;
  CoordToChar Result2;

  i64 curY = 0;

  for (auto &Line : Mapping) {
    i64 curX  = 0;
    for (char c : Line) {
      Result1[c] = Coord2{curX,curY};
      Result2[Coord2{curX,curY}] = c;
      ++curX;
    }
    ++curY;
  }

  return make_pair(Result1,Result2);
}

auto [NumCharMap, NumCoordMap] = buildMaps(NumPad);
auto [KeyCharMap, KeyCoordMap] = buildMaps(DirectionalPad);

using CacheType = pair<string, i64>;

map<CacheType, i64> MoveLengthsByLevel;
i64 getMinRequiredMovesDirPad(char CurrentPos, char ToPress, i64 Level);

i64 getTotalSequenceLength(string &Sequence, i64 Level) {
  i64 Result = 0;
  char Current = 'A';

  for (auto Next : Sequence) {
    Result += getMinRequiredMovesDirPad(Current, Next, Level);
    Current = Next;
  }

  return Result;
}
/*
 * Calculate fewest amount of moves required to move from Start to Target and press 'A'
 * to confirm press on target
 */
i64 getMinRequiredMovesDirPad(char CurrentPos, char ToPress, i64 Level) {
  CacheType Key = make_pair(string{CurrentPos, ToPress}, Level);

  if (MoveLengthsByLevel.contains(Key)) {
    return MoveLengthsByLevel[Key];
  }

  Coord2 Distance = KeyCharMap[ToPress] - KeyCharMap[CurrentPos];

  if (Level == 0) {
    // Level 0 -> Human operator
    // Simply the number of vertical presses + number of horizontal presses,
    // as we can ignore side constraints about moves becoming suboptimal on higher
    // levels.

    // Add 1, as we need to move the arm and then also press "A" afterward
    MoveLengthsByLevel[Key] = Distance.manhattanLength() + 1;
    return MoveLengthsByLevel[Key];
  }

  i64 hDiff = Distance.x();
  i64 vDiff = Distance.y();

  string hMovement = string(abs(hDiff), hDiff<0?'<':'>');
  string vMovement = string(abs(vDiff), vDiff<0?'^':'v');

  if (hDiff == 0 || vDiff == 0) {
    // Only one possible sequence
    string Sequence = hMovement + vMovement + "A";
    MoveLengthsByLevel[Key] = getTotalSequenceLength(Sequence, Level - 1);

    return MoveLengthsByLevel[Key];
  }

  // There may be two possible sequences
  // However first check if one of them leads to an undesirable state
  if (KeyCharMap[CurrentPos] + Coord2{hDiff,0} == KeyCharMap[' ']) {
    // Only one possibility, move vertically first
    string Sequence = vMovement + hMovement + "A";
    MoveLengthsByLevel[Key] = getTotalSequenceLength(Sequence, Level - 1);

    return MoveLengthsByLevel[Key];
  }

  if (KeyCharMap[CurrentPos] + Coord2{0, vDiff} == KeyCharMap[' ']) {
    // Only one possibility, move horizontally first
    string Sequence = hMovement + vMovement + "A";
    MoveLengthsByLevel[Key] = getTotalSequenceLength(Sequence, Level - 1);

    return MoveLengthsByLevel[Key];
  }

  // There are two possibilities to check
  string Sequence1 = hMovement + vMovement + "A";
  string Sequence2 = vMovement + hMovement + "A";

  MoveLengthsByLevel[Key] = min(
    getTotalSequenceLength(Sequence1, Level - 1),
    getTotalSequenceLength(Sequence2, Level - 1)
    );

  return MoveLengthsByLevel[Key];
}

i64 getMinRequiredMovesNumPad(char CurrentLoc, char ToPress, i64 NumRobots) {
  NumRobots -= 1;
  Coord2 Diff = NumCharMap[ToPress] - NumCharMap[CurrentLoc];

  i64 hDiff = Diff.x();
  i64 vDiff = Diff.y();

  string hMovement = string(abs(hDiff), hDiff<0?'<':'>');
  string vMovement = string(abs(vDiff), vDiff<0?'^':'v');

  if (hDiff == 0 || vDiff == 0) {
    // Only one possible sequence
    string Sequence = hMovement + vMovement + "A";

    return getTotalSequenceLength(Sequence, NumRobots);
  }

  // There may be two possible sequences
  // However first check if one of them leads to an undesirable state
  if (NumCharMap[CurrentLoc] + Coord2{hDiff,0} == NumCharMap[' ']) {
    // Only one possibility, move vertically first
    string Sequence = vMovement + hMovement + "A";
    return getTotalSequenceLength(Sequence, NumRobots);
  }

  if (NumCharMap[CurrentLoc] + Coord2{0, vDiff} == NumCharMap[' ']) {
    // Only one possibility, move horizontally first
    string Sequence = hMovement + vMovement + "A";

    return getTotalSequenceLength(Sequence, NumRobots);
  }

  // There are two possibilities to check
  string Sequence1 = hMovement + vMovement + "A";
  string Sequence2 = vMovement + hMovement + "A";

  return min(
    getTotalSequenceLength(Sequence1, NumRobots),
    getTotalSequenceLength(Sequence2, NumRobots)
    );
}

void part1() {
  auto Inputs = IO::fromCin();

  i64 Res = 0;
  for (string &Line : Inputs) {
    stringstream ss{Line};
    i64 Multiplier;

    ss >> Multiplier;

    char Current = 'A';

    i64 NumPresses = 0;
    for (char Next : Line) {
      NumPresses += getMinRequiredMovesNumPad(Current, Next, 2);
      Current = Next;
    }

    cout << Line << ": " << NumPresses << "*" << Multiplier << "(" << NumPresses*Multiplier << ")" << endl;

    Res += NumPresses*Multiplier;
  }

  cout << Res << endl;
}

void part2() {
  auto Inputs = IO::fromCin();

  i64 Res = 0;
  for (string &Line : Inputs) {
    stringstream ss{Line};
    i64 Multiplier;

    ss >> Multiplier;

    char Current = 'A';

    i64 NumPresses = 0;
    for (char Next : Line) {
      NumPresses += getMinRequiredMovesNumPad(Current, Next, 25);
      Current = Next;
    }

    cout << Line << ": " << NumPresses << "*" << Multiplier << "(" << NumPresses*Multiplier << ")" << endl;

    Res += NumPresses*Multiplier;
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