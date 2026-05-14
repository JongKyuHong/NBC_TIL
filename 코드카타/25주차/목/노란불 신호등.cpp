// https://school.programmers.co.kr/learn/courses/30/lessons/468371

#include <string>
#include <vector>
#include <climits>

using namespace std;

long long gcd(long long a, long long b) {
    return b == 0 ? a : gcd(b, a % b);
}

long long lcm(long long a, long long b) {
    return a / gcd(a, b) * b;
}

int solution(vector<vector<int>> signals) {
    // 그러면 time을 몇까지 가져갔을때 겹치는게 없는지 확정할 수 있는가
    int N = signals.size();
    long long time = 0;
    long long totalCycle = 1;
    for (int i = 0; i < N; i++) {
        long long cycle = signals[i][0] + signals[i][1] + signals[i][2];
        totalCycle = lcm(totalCycle, cycle);
    }
    
    // 현재 색깔 인덱스, 남은시간
    vector<pair<int, int>> remains(N);
    for (int i = 0; i < N; ++i){
        remains[i].first = 0;
        remains[i].second = signals[i][0];
    }
    
    while (time < totalCycle){
        time++;
        bool flag = true;
        for (int i = 0; i < N; ++i){
            // 현 색깔
            if (remains[i].first != 1){
                flag = false;
            }
            remains[i].second -= 1;
            if (remains[i].second == 0){
                int newIndex = (remains[i].first+1)%3;
                remains[i].first = newIndex;
                remains[i].second = signals[i][newIndex];
            }
        }
        if (flag){
            break;
        }
    }
    
    return time == totalCycle ? -1 : time;
}