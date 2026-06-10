// https://school.programmers.co.kr/learn/courses/30/lessons/17679?language=cpp

#include <string>
#include <vector>
#include <iostream>

using namespace std;


int solution(int m, int n, vector<string> board) {
    int answer = 0;
    
    // 순회 턴, 삭제 턴 나눔
    int index = 0;
    
    // while문 탈출 플래그
    bool flag;
    while (true){
        flag = true;
        vector<vector<bool>> visited(m, vector<bool>(n, false));
        for (int i = 0; i < m - 1; i++){
            for (int j = 0; j < n - 1; j++){
                if (board[i][j] != ' ' && board[i][j] == board[i+1][j] && board[i][j] == board[i][j+1] && board[i][j] == board[i+1][j+1]){
                    visited[i][j] = true;
                    visited[i][j+1] = true;
                    visited[i+1][j] = true;
                    visited[i+1][j+1] = true;
                    flag = false;
                }
            }
        }
        if (flag) break;
        // 개수세고 비우기
        for (int i = 0; i < m; i++){
            for (int j = 0; j < n; j++){
                if (visited[i][j]) {
                    answer++;
                    board[i][j] = ' ';
                }
            }
        }
        // 아래로 당기기
        for (int j = 0; j < n; j++){
            int index = m-1;
            int blankIndexStart = -1;
            while (index >= 0){
                if (blankIndexStart == -1 && board[index][j] == ' '){
                    blankIndexStart = index;
                }
                if (blankIndexStart != -1 && board[index][j] != ' '){
                    board[blankIndexStart--][j] = board[index][j];    
                    board[index][j] = ' ';
                }
                index--;
            }
        }
    }
    
    return answer;
}