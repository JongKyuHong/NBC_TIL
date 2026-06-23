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
    
    if (row <= 1 || col <= 1){
        for (int i = 0; i < row; i++){
            for (int j = 0; j < col; j++){
                if (board[i][j] == 1) return 1;
            }
        }
        return 0;
    }

    for (int i = 1; i < row; i++){
        for (int j = 1; j < col; j++){
            if (board[i][j] == 1){
                board[i][j] = min({board[i-1][j], board[i][j-1], board[i-1][j-1]}) + 1;
                maxValue = max(maxValue, board[i][j]);
            }
        }
    }

    return maxValue * maxValue;
}