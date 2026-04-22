// https://school.programmers.co.kr/learn/courses/30/lessons/1845?language=cpp

#include <vector>
#include <unordered_set>
using namespace std;

int solution(vector<int> nums)
{
    int answer = 0;
    unordered_set<int> us;
    
    for (int num : nums){
        us.insert(num);
    }
    
    int ussize = us.size();
    int n = nums.size();
    
    return ussize > n / 2 ?  n / 2 : ussize;
}