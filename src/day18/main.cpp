#include <algorithm>
#include "lib.h"

using namespace std;
using transform::Coord2;
using Map = map<Coord2,bool>;

auto parseInput() {
  V<Coord2> Walls;

  for (string line;getline(cin,line);) {
    V<i64> C = util::fromString<i64>(util::split(line,","));

    Walls+= Coord2{C[0],C[1]};
  }

  return Walls;
}

V<Coord2> Dirs = {Coord2{1,0},Coord2{0,1},Coord2{-1,0},Coord2{0,-1}};

i64 Heuristic(const Coord2 &Current,const Coord2 &Target) {
  return (Current-Target).manhattanLength();
}

i64 getShortestPath(Coord2 &Start, Coord2 &Finish, Map &Walls, Coord2 &GridSize) {
  Map Visited;

  using Entry = pair<i64, Coord2>;
  priority_queue<Entry, V<Entry>, std::greater<Entry>> Open;

  map<Coord2, util::DefaultI64<maxval<i64>()>> Cost;
  map<Coord2, util::DefaultI64<maxval<i64>()>> heuristic;

  Cost[Start] = 0;
  heuristic[Start] = Heuristic(Start, Finish);
  Open += make_pair(Heuristic(Start,Finish),Start);

  while (!Open.empty()) {
    auto [HCost, Pos] = Open.top();
    Open.pop();

    if (Visited[Pos]) {
      continue;
    }

    Visited[Pos] = true;

    if (Pos == Finish) {
      return Cost[Pos];
    }

    if (HCost > heuristic[Pos] ) {
      continue;
    }

    for (auto &Dir : Dirs) {
      Coord2 Next = Pos + Dir;

      if (Next.x() < 0 || Next.x()>=GridSize.x() || Next.y() < 0 || Next.y() >= GridSize.y()) {
        continue;
      }

      if (Walls[Next]) {
        continue;
      }

      i64 NextH = Cost[Pos] + 1 + Heuristic(Next,Finish);
      i64 NextC = Cost[Pos] + 1;

      if (Cost[Next]< NextC ) {
        continue;
      }

      if (heuristic[Next] < NextH ) {
        continue;
      }

      Cost[Next] = NextC;
      heuristic[Next] = NextH;

      Open += make_pair(NextH, Next);
    }
  }
  return -1;
}

void part1(){
  auto Bytes = parseInput();

  Map Walls;
  Coord2 GridSize{71,71};

  for (st Idx = 0; Idx<1024; ++Idx) {
    Walls[Bytes[Idx]] = true;
  }

  Coord2 Start = Coord2{0,0};
  Coord2 End = Coord2{70,70};

  i64 Cost = getShortestPath(Start, End, Walls, GridSize);

  cout << Cost << endl;
}

void part2(){

  auto Bytes = parseInput();

  Map Walls;
  Coord2 GridSize{71,71};

  Coord2 Start = Coord2{0,0};
  Coord2 End = Coord2{70,70};

  for (auto &Pos : Bytes) {
    Walls[Pos] = true;
    i64 Cost = getShortestPath(Start, End, Walls, GridSize);

    if (Cost==-1) {
      cout << Pos.x() << "," << Pos.y() << endl;
      return;
    }
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