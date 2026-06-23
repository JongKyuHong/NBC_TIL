// https://school.programmers.co.kr/learn/courses/30/lessons/12905?language=cpp

#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>
#include <cmath>
using namespace std;

int solution(vector<vector<int>> board)
{
    int maxValue = 0;
    int row = board.size();
    int col = board[0].size();
    vector<vector<int>> dp(row, vector<int>(col, 0));

    for (int i = 0; i < row; i++){
        dp[i][0] = board[i][0];
        if (board[i][0] == 1) maxValue = 1;
    }
    for (int i = 0; i < col; i++){
        dp[0][i] = board[0][i];
        if (board[0][i] == 1) maxValue = 1;
    }
    
    for (int i = 1; i < row; i++){
        for (int j = 1; j < col; j++){
            if (board[i][j] == 1){
                dp[i][j] = min({dp[i-1][j], dp[i][j-1], dp[i-1][j-1]}) + 1;
                maxValue = max(maxValue, dp[i][j]);
            }
        }
    }

    return maxValue * maxValue;
}