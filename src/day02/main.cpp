#include <algorithm>
#include "lib.h"

using namespace std;
void part1(){
    i64 safe = 0;

    for(string line;getline(cin,line);)
    {
        stringstream ss(line);
        i64 current, last;
        ss >> last;
        ss >> current;

        bool inc = false;

        if(current==last)
        {
            continue;
        }

        if(current>last)
        {
            inc = true;
        }

        if(abs(last-current)<1 || abs(last-current)>3)
        {
            continue;
        }

        last = current;

        bool isSafe = true;
        while(ss>>current)
        {
            if(inc)
            {
                if(current<=last)
                {
                    isSafe = false;
                    break;
                }
            } else
            {
                if(current>=last)
                {
                    isSafe = false;
                    break;
                }
            }

            if(abs(last-current)<1 || abs(last-current)>3)
            {
                isSafe = false;
                break;
            }

            last = current;
        }

        if(isSafe)
        {
            ++safe;
        }
    }

    cout << safe << endl;
}

bool checkSafeInc(i64 n1, i64 n2)
{
    if(n2<=n1)
    {
        return false;
    }

    auto diff = abs(n1-n2);
    if(diff<1 || diff>3)
    {
        return false;
    }

    return true;
}

bool checkSafeDec(i64 n1, i64 n2)
{
    if(n2>=n1)
    {
        return false;
    }

    auto diff = abs(n1-n2);
    if(diff<1 || diff>3)
    {
        return false;
    }

    return true;
}

bool isSafeInc(V<i64> &nums)
{
    for(st idx=1;idx<nums.size();++idx)
    {
        if(nums[idx]<=nums[idx-1])
        {
            return false;
        }

        auto diff = abs(nums[idx]-nums[idx-1]);

        if(diff<1 || diff>3)
        {
            return false;
        }
    }

    return true;
}

bool isSafeDec(V<i64> &nums)
{
    for(st idx=1;idx<nums.size();++idx)
    {
        if(nums[idx]>=nums[idx-1])
        {
            return false;
        }

        auto diff = abs(nums[idx]-nums[idx-1]);

        if(diff<1 || diff>3)
        {
            return false;
        }
    }

    return true;
}

#if 0
bool isSafeInc(V<i64> & nums)
{
    V<ui64> dp(nums.size());

    dp[0] = 0;

    if(nums[1]<=nums[0])
    {
        return false;
    }

    auto diff = abs(nums[0]-nums[1]);
    if(diff<1 || diff>3)
    {
        dp[1] = 1;
    }
    else
    {
        dp[1] = 0;
    }

    for(st idx = 2;idx<nums.size();++idx)
    {
        if(checkSafeInc(nums[idx-1],nums[idx]))
        {
            dp[idx] = dp[idx-1];
        } else if(checkSafeInc(nums[idx-2],nums[idx]))
        {
            dp[idx] = dp[idx-1] + 1;
        } else
        {
            dp[idx] = 1000;
        }
    }

    return dp.back() <= 1;
}

bool isSafeDec(V<i64> & nums)
{
    V<ui64> dp(nums.size());

    dp[0] = 0;

    if(nums[1]>=nums[0])
    {
        return false;
    }

    auto diff = abs(nums[0]-nums[1]);
    if(diff<1 || diff>3)
    {
        dp[1] = 1;
    }
    else
    {
        dp[1] = 0;
    }

    for(st idx = 2;idx<nums.size();++idx)
    {
        if(checkSafeDec(nums[idx-1],nums[idx]))
        {
            dp[idx] = dp[idx-1];
        } else if(checkSafeDec(nums[idx-2],nums[idx]))
        {
            dp[idx] = dp[idx-1] + 1;
        } else
        {
            dp[idx] = 1000;
        }
    }

    return dp.back() <= 1;
}
#endif

void part2(){
    i64 safe = 0;
    for(string line;getline(cin,line);)
    {
        stringstream ss(line);

        V<i64> numbers;

        i64 num;

        while(ss>>num)
        {
            numbers += num;
        }

        if(isSafeInc(numbers) || isSafeDec(numbers))
        {
            ++safe;
            continue;
        }

        for(st idx = 0; idx< numbers.size();++idx)
        {
            V<i64> numbers2 = numbers;

            numbers2.erase(std::next(numbers2.begin(),idx));

            if(isSafeInc(numbers2) || isSafeDec(numbers2))
            {
                ++safe;
                break;
            }
        }
    }

    cout << safe << endl;
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