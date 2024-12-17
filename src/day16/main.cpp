#include <algorithm>
#include "lib.h"

using namespace std;
using transform::Coord2;

auto parseInput() {
  map<Coord2,bool> Walls;

  i64 maxX = 0;
  i64 maxY = 0;
  Coord2 StartPos, EndPos;

  for (string line; getline(cin,line);++maxY) {
    i64 curX = 0;

    for (char c : line) {
      switch (c) {
      case '#':
        Walls[Coord2{curX,maxY}] = true;
        break;
      case 'S':
        StartPos = Coord2{curX,maxY};
        break;
      case 'E':
        EndPos = Coord2{curX,maxY};
      }
      ++curX;
    }


    maxX = max(maxX,curX);
  }

  return make_tuple(Walls, StartPos, EndPos, Coord2{maxX,maxY});
}

array Dirs = {Coord2{1,0},Coord2{0,1},Coord2{-1,0},Coord2{0,-1}};

void part1(){
  auto [Walls, StartPos, EndPos, GridSize] = parseInput();

  map<pair<Coord2,i64>, util::DefaultI64<maxval<i64>()>> Costs;

  Costs[make_pair(StartPos,0)] = 0;

  priority_queue<tuple<i64,Coord2,i64>, std::vector<tuple<i64,Coord2,i64>>, std::greater<tuple<i64,Coord2,i64>>> Open;

  Open.push(make_tuple(0,StartPos,0));

  while (!Open.empty()) {
    auto [Cost, Pos, Dir] = Open.top();
    Open.pop();

    //cout << Pos.x() << "|" << Pos.y() << endl;

    if (Pos == EndPos) {
      cout << Cost << endl;
      return;
    }

    if (Cost > Costs[make_pair(Pos,Dir)]) {
      continue;
    }

    V<pair<st,i64>> DirCosts = {{Dir,1},{(Dir+1)%4,1001}, {(Dir+3)%4,1001}};

    for (auto [DirIdx, ExtraCost] : DirCosts) {
      if (Walls[Pos+Dirs[DirIdx]]) {
        continue;
      }

      if (Costs[make_pair(Pos+Dirs[DirIdx], DirIdx)] < Cost+ExtraCost) {
        continue;
      }

      Costs[make_pair(Pos+Dirs[DirIdx], DirIdx)] = Cost+ExtraCost;
      Open += make_tuple(Cost+ExtraCost,Pos+Dirs[DirIdx], DirIdx);
    }
  }
}

struct PathNode {
  Coord2 Pos;
  shared_ptr<PathNode> Previous;

  PathNode(const Coord2 &P, const shared_ptr<PathNode> & Prev)
    :Pos(P),Previous(Prev){};
};

void part2(){
  auto [Walls, StartPos, EndPos, GridSize] = parseInput();

  map<pair<Coord2,i64>, util::DefaultI64<maxval<i64>()>> Costs;

  Costs[make_pair(StartPos,0)] = 0;

  using TpType = tuple<i64,shared_ptr<PathNode>,i64>;
  priority_queue<TpType, std::vector<TpType>, std::greater<TpType>> Open;

  shared_ptr<PathNode> StartNode = make_shared<PathNode>(StartPos,nullptr);
  Open.push(make_tuple(0,StartNode,0));

  i64 MaxCost = maxval<i64>();

  set<Coord2> Visited;

  while (!Open.empty()) {
    auto [Cost, Node, Dir] = Open.top();
    Open.pop();

    if (Cost>MaxCost) {
      break;
    }

    auto Pos = Node->Pos;
    //cout << Pos.x() << "|" << Pos.y() << endl;

    if (Pos == EndPos) {
      MaxCost = Cost;
      while (Node) {
        Visited += Node->Pos;
        Node = Node->Previous;
      }
      continue;
    }

    if (Cost > Costs[make_pair(Pos,Dir)]) {
      continue;
    }

    V<pair<st,i64>> DirCosts = {{Dir,1},{(Dir+1)%4,1001}, {(Dir+3)%4,1001}};

    for (auto [DirIdx, ExtraCost] : DirCosts) {
      if (Walls[Pos+Dirs[DirIdx]]) {
        continue;
      }

      if (Costs[make_pair(Pos+Dirs[DirIdx], DirIdx)] < Cost+ExtraCost) {
        continue;
      }

      Costs[make_pair(Pos+Dirs[DirIdx], DirIdx)] = Cost+ExtraCost;
      auto NewNode = make_shared<PathNode>(Pos+Dirs[DirIdx],Node);
      Open += make_tuple(Cost+ExtraCost,NewNode, DirIdx);
    }
  }

  cout << Visited.size() << endl;
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