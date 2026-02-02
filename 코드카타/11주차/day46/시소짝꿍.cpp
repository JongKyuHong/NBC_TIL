#include <string>
#include <vector>
#include <algorithm>
#include <unordered_map>
#include <iostream>
using namespace std;

long long solution(vector<int> weights) {
    long long answer = 0;
    unordered_map<int, int> um;
    sort(weights.begin(), weights.end());

    for (int i = 0; i < weights.size(); i++){
        int target = weights[i];
        if (um.count(target)) answer += um[target];
        if ((target*2)%3 == 0 && um.count(target*2/3)) answer += um[target*2/3];
        if (target%2 == 0 && um.count(target*1/2)) answer += um[target*1/2];
        if ((target*3)%4 == 0 && um.count(target*3/4)) answer += um[target*3/4];
        um[target]++;
    }

    return answer;
}