#include <string>
#include <vector>
#include <iostream>

using namespace std;

bool isSafe(int row, int col, vector<vector<int>>& visited, int n){
    for (int j = 0; j < n; j++){
        if (row-j >= 0){
            if (col - j >= 0){
                if (visited[row-j][col-j]){
                    return false;
                }        
            }
            if (visited[row-j][col]){
                return false;
            }
            if (col + j < n){
                if (visited[row-j][col+j]){
                    return false;
                }
            }
        }
    }
    return true;
}

int answer;
void dfs(int row, vector<vector<int>>& visited, int n){
    if (row == n-1){
        answer++;
        return;
    }
    
    for (int i = 0; i < n; i++){
        if (isSafe(row+1, i, visited, n)){
            visited[row+1][i] = 1;
            dfs(row+1, visited, n);
            visited[row+1][i] = 0;
        }
    }
}

int solution(int n) {
    answer = 0;
    
    // 위쪽행과 행-행 열-열이 같으면 대각에 있음 이거는 안됨
    // 위쪽행과 열이 같음 이것도 안됨
    vector<vector<int>> visited(n, vector<int>(n, 0));
    for (int i = 0; i < n; i++){
        visited[0][i] = 1;
        dfs(0, visited, n);
        visited[0][i] = 0;
    }
    
    
    return answer;
}