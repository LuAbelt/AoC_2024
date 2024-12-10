#include <algorithm>
#include "lib.h"

using namespace std;
void part1() {
  string line;
  getline(cin,line);

  V<i64> FileIds;

  for (st idx = 0; idx<line.length();++idx) {
    if (idx%2==0) {
      i64 NumEntries = line[idx] - '0';

      while (NumEntries) {
        FileIds += idx / 2;
        --NumEntries;
      }
    } else {
      i64 NumEntries = line[idx] - '0';

      while (NumEntries) {
        FileIds += -1;
        --NumEntries;
      }
    }
  }

  st FrontIdx = 0;
  st BackIdx = FileIds.size()-1;

  while (FrontIdx<BackIdx) {
    if (FileIds[FrontIdx] == -1) {
      while (FileIds[BackIdx] == -1 && FrontIdx < BackIdx) {
        --BackIdx;
      }
      swap(FileIds[FrontIdx], FileIds[BackIdx]);
    }

    ++FrontIdx;
  }

  i64 Res = 0;

  st idx = 0;
  for (auto ID : FileIds) {
    if (ID==-1) {
      break;
    }

    Res += idx*ID;

    ++idx;
  }

  cout << Res << endl;
}

void part1Wrong(){
  string line;
  getline(cin,line);

  st FrontIdx = 0;
  st BackIdx = line.length()-1;

  if ((BackIdx % 2) == 1) {
    --BackIdx;
  }

  st OutputIdx = 0;
  st NumLeftBack = line[BackIdx] - '0';

  i64 Result = 0;

  while (FrontIdx < BackIdx) {
    i64 Num = line[FrontIdx] - '0';
    if (FrontIdx % 2 == 0 ) {
      i64 FileId = FrontIdx / 2;
      for (int i = 0;i<Num;++i) {
        Result += FileId*OutputIdx;
        //cout << FileId;
        ++OutputIdx;
      }
    } else {
      i64 FileId = BackIdx / 2;
      while (NumLeftBack == 0) {
        BackIdx -= 2;
        FileId = BackIdx / 2;
        NumLeftBack = line[BackIdx] - '0';
      }
      while (Num && FrontIdx < BackIdx) {
        Result += FileId * OutputIdx;
        //cout << FileId;
        ++OutputIdx;
        --NumLeftBack;
        --Num;
        while (NumLeftBack == 0) {
          BackIdx -= 2;
          FileId = BackIdx / 2;
          NumLeftBack = line[BackIdx] - '0';
        }
      }
    }

    ++FrontIdx;
  }

  while (NumLeftBack) {
    Result += (BackIdx/2)*OutputIdx;
    ++OutputIdx;
    --NumLeftBack;
    //cout << (BackIdx/2);
  }

  /*
  if (FrontIdx%2 == 0) {
    i64 Num = line[FrontIdx] - '0';
    while (Num) {
      Result += (FrontIdx/2)*OutputIdx;
      ++OutputIdx;
      --Num;
    }
  }
  */

  cout << endl << Result << endl;
}


struct File {
  i64 ID;
  i64 StartIdx;
  i64 Length;
};

void part2(){
  string Input;

  getline(cin,Input);

  V<File> Files;
  V<File> EmptySpaces;

  i64 OutIdx = 0;
  for (st Idx = 0; Idx<Input.length();++Idx) {
    i64 Length = Input[Idx] - '0';
    if (Idx % 2 == 0) {
      i64 FileId = Idx / 2;
      Files += File{FileId,OutIdx,Length};
    } else {
      EmptySpaces += File{-1,OutIdx,Length};
    }
    OutIdx += Length;
  }

  ranges::reverse(Files);

  for (File& File : Files ) {
    for (auto &Empty : EmptySpaces) {
      if (Empty.StartIdx > File.StartIdx) {
        break;
      }

      if (Empty.Length < File.Length) {
        continue;
      }

      File.StartIdx = Empty.StartIdx;
      Empty.StartIdx += File.Length;
      Empty.Length -= File.Length;
    }
  }

  i64 Res = 0;
  for (auto &File : Files) {
    for (i64 Idx = 0; Idx<File.Length; ++Idx) {
      Res += (File.StartIdx+Idx)*File.ID;
    }
  }

  cout << Res;
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