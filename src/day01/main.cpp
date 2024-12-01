#include <algorithm>
#include "lib.h"

using namespace std;

pair<V<i64>,V<i64>> parseInputs() {
    V<i64> input1, input2;

    for(string line;getline(cin,line);)
    {
        stringstream ss(line);

        i64 num1, num2;

        ss >> num1 >> num2;

        input1.push_back(num1);
        input2.push_back(num2);
    }

    return make_pair(input1,input2);
}

void part1(){
    auto input = parseInputs();
    V<i64> list1 = input.first;
    V<i64> list2 = input.second;

    sort(list1.begin(),list1.end());
    sort(list2.begin(),list2.end());

    i64 res = 0;

    for(int i=0;i<list1.size();i++)
    {
        i64 distance = abs(list1[i] - list2[i]);

        res += distance;
    }

    cout << res << endl;
}

void part2(){
    auto input = parseInputs();
    V<i64> list1 = input.first;
    V<i64> list2 = input.second;

    sort(list1.begin(),list1.end());
    sort(list2.begin(),list2.end());

    i64 res = 0;

    map<i64,i64> counts;
    for(auto num : list2)
    {
        counts[num] += 1;
    }

    for(auto num : list1)
    {
        res += counts[num]*num;
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