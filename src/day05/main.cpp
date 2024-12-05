#include <algorithm>
#include "lib.h"

using namespace std;

auto parseInput() {
  std::vector<pair<i64,i64>> pairs;
  for (string line; getline(cin, line);) {
    if (!line.contains('|')) {
      break;
    }
    st sep = line.find('|');
    i64 num1 = atoll(line.substr(0, sep).c_str());
    i64 num2 = atoll(line.substr(sep+1).c_str());

    pairs.push_back(make_pair(num1,num2));
  }

  V<V<i64>> pages;

  for (string line; getline(cin, line);) {
    pages.push_back(util::fromString<i64>(util::split(line,",")));
  }

  return make_pair(pairs,pages);
}

bool isValid(const V<i64> &line, map<i64,set<i64>> &Before, map<i64,set<i64>> &After) {
  for (int i = 0;i<line.size();++i) {
    // Check all elements before that

    for (int j = 0; j<i; ++j) {
      if (After[line[j]].contains(line[i])) {
        return false;
      }
    }

    for (int j = i+1; j<line.size();++j) {
      if (Before[line[j]].contains(line[i])) {
        return false;
      }
    }
  }

  return true;
}

void part1(){
  auto [pairs, pages] = parseInput();

  map<i64,set<i64>> befores;
  map<i64,set<i64>> afters;

  for (const auto [first,second] : pairs) {
    befores[first] += second;
    afters[second] += first;
  }

  i64 res = 0;
  for (const auto& Order : pages) {
    if (isValid(Order,befores,afters)) {
      res += Order[Order.size()/2];
      //cout << "Valid" << endl;
    } else {
      //cout << "Invalid" << endl;
    }
  }

  cout << res << endl;
}

auto addId(i64 Num, map<i64,i64> &Id2Num, map<i64,i64> &Num2Id, st &CurId) {
  if (Num2Id.find(Num) == Num2Id.end()) {
    Id2Num[CurId] = Num;
    Num2Id[Num] = CurId;
    ++CurId;
  }
}

auto buildTopoGraph(V<pair<i64,i64>> &Orders) {
  map<i64,i64> Id2Num;
  map<i64,i64> Num2Id;

  // Build IDs
  st CurId = 0;

  for (auto &[first, second] : Orders) {
    addId(first,Id2Num,Num2Id,CurId);
    addId(second,Id2Num,Num2Id,CurId);
  }

  V<V<i64>> adj(CurId);
  for (auto &[first, second] : Orders) {
    auto Id1 = Num2Id[first];
    auto Id2 = Num2Id[second];

    adj[Id1] += Id2;
  }

  return make_tuple(adj, Id2Num, Num2Id);
}

bool VecEqual(V<i64> &V1, V<i64> &V2) {
  if (V1.size() != V2.size()) {
    cout << "ERROR" << endl;
    return false;
  }

  for (int i = 0; i<V1.size(); ++i) {
    if (V1[i] != V2[i]) {
      return false;
    }
  }

  return true;
}

void part2(){
  i64 res = 0;
  auto [pairs, pages] = parseInput();

  auto [Adj, Id2Num, Num2Id] = buildTopoGraph(pairs);

  for (auto PageOrder : pages) {
    V<i64> IdVec;

    set<i64> PageSet(PageOrder.begin(), PageOrder.end());

    for (auto Page : PageOrder) {
      IdVec += Num2Id[Page];
    }

    auto TopoOrderID = graphs::topological_sort(Adj,IdVec);

    V<i64> CorrectOrder;

    for (auto ID : TopoOrderID) {
      auto Num = Id2Num[ID];
      if (!PageSet.contains(Num)) {
        continue;
      }

      CorrectOrder += Num;
    }

    if (!VecEqual(CorrectOrder, PageOrder)) {
      res += CorrectOrder[CorrectOrder.size()/2];
      //cout << "Invalid" << endl;
    } else {
      //cout << "Valid" << endl;
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