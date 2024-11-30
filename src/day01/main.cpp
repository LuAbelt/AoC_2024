#include <algorithm>
#include "lib.h"

using namespace std;
void part1(){
    int a,b,c;

    std::cin>>a>>b>>c;

    std::cout<<"a: " << a << "; b: "<< b << "; c: "<<c;
}

void part2(){
}

int main(int argc, char* argv[]) {
    if(argc<2)
    {
        std::cout << "Wrong usage!" << std::endl;
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