#include <algorithm>
#include "lib.h"

using namespace std;

auto parseInput() {
  set<string> Nodes;
  map<string, set<string>> Neighbours;
  for (auto &Line : IO::fromCin()) {
    auto Parts = util::split(Line, "-");
    string Left = Parts[0];
    string Right = Parts[1];

    Nodes += Left;
    Nodes += Right;
    Neighbours[Left] += Right;
    Neighbours[Right] += Left;
  }

  return make_tuple(Nodes, Neighbours);
}

void part1(){
  auto [Nodes, Neighbours] = parseInput();

  set<set<string>> Result;
  for (const string &Node1 : Nodes) {
    for (const string &Node2 : Neighbours[Node1]) {
      for (const string &Node3 : Neighbours[Node2]) {
        if (Node3 == Node1) {
          continue;
        }

        if (!Neighbours[Node1].contains(Node3)) {
          continue;
        }

        if (!(Node1.starts_with('t')||Node2.starts_with('t') || Node3.starts_with('t'))) {
          continue;
        }

        Result += set{Node1,Node2,Node3};
      }
    }
  }

  cout << Result.size() << endl;
}

set<string> findMaxClique(const string &StartNode, map<string, set<string>> &Neighbours) {
  set<string> Result{StartNode};

  set<set<string>> Candidates;
  map<set<string>, bool> Added;

  for (auto& Neighbour: Neighbours[StartNode]) {
    Candidates += set{StartNode, Neighbour};
    Added[set{StartNode, Neighbour}] = true;
  }



  while (!Candidates.empty()) {
    set<set<string>> NewCandidates;

    for (auto& Candidate : Candidates){
      Result = Candidate;
      set<string> CommonNeighbours = Neighbours[*Result.begin()];

      for (auto& Node : Result) {
        V<string> Intersection;
        ranges::set_intersection(CommonNeighbours, Neighbours[Node],
          back_inserter(Intersection));

        CommonNeighbours = set(Intersection.begin(),Intersection.end());
      }

      for (auto &NewNode : CommonNeighbours) {
        if (Result.contains(NewNode)) {
          continue;
        }

        set<string> NewCandidate = Result;
        NewCandidate += NewNode;
        if (Added[NewCandidate]) {
          continue;
        }

        Added[NewCandidate] = true;

        NewCandidates.emplace(NewCandidate);
      }
    }

    Candidates = NewCandidates;
  }

  return Result;
}

void part2() {
  auto [Nodes, Neighbours] = parseInput();

  cout << "Num total nodes: " << Nodes.size() << endl;

  set<string> CurMaxClique{};
  i64 N = 0;

  for (auto &Node : Nodes) {
    cout << "Checking Node Number " << N++ << endl;
    auto NewClique = findMaxClique(Node,Neighbours);

    if (NewClique.size()>CurMaxClique.size()) {
      CurMaxClique = NewClique;
    }
  }

  for (auto &Node : CurMaxClique) {
    cout << Node << ",";
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