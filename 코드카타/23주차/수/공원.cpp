// https://school.programmers.co.kr/learn/courses/30/lessons/340198

#include <string>
#include <vector>
#include <algorithm>

using namespace std;

int solution(vector<int> mats, vector<vector<string>> park) {
    int answer = -1;
    
    int N = park.size();
    int M = park[0].size();
    vector<vector<int>> dp(N, vector<int>(M, 0));
    
    for (int i = 0; i < N; ++i){
        for (int j = 0; j < M; ++j){
            if (park[i][j] == "-1") {
                if (i == 0 || j == 0) {
                    dp[i][j] = 1;
                    continue;
                }
                dp[i][j] = min({dp[i-1][j], dp[i][j-1], dp[i-1][j-1]}) + 1;       
            }
        }
    }
    
    int maximum = 0;
    for (int i = 0; i < N; ++i){
        for (int j = 0; j < M; ++j){
            maximum = max(maximum, dp[i][j]);
        }
    }
    
    for (int mat : mats){
        if (maximum >= mat){
            answer = max(answer, mat);
        }
    }
    
    return answer;
}