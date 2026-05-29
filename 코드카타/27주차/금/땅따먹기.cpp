// https://school.programmers.co.kr/learn/courses/30/lessons/12913?language=cpp

#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

int solution(vector<vector<int> > land)
{
    int answer = 0;
    int N = land.size();
    vector<vector<int>> dp(N, vector<int>(4, 0));
    
    dp[0][0] = land[0][0];
    dp[0][1] = land[0][1];
    dp[0][2] = land[0][2];
    dp[0][3] = land[0][3];
    
    
    for (int i = 1; i < N; i++){
        dp[i][0] = max({dp[i-1][1], dp[i-1][2], dp[i-1][3]}) + land[i][0];
        dp[i][1] = max({dp[i-1][0], dp[i-1][2], dp[i-1][3]}) + land[i][1];
        dp[i][2] = max({dp[i-1][0], dp[i-1][1], dp[i-1][3]}) + land[i][2];
        dp[i][3] = max({dp[i-1][0], dp[i-1][1], dp[i-1][2]}) + land[i][3];
    }
    
    answer = max({dp[N-1][0],dp[N-1][1],dp[N-1][2],dp[N-1][3]});
    
    return answer;
}