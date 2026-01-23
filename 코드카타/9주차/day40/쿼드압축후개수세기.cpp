#include <string>
#include <vector>
#include <iostream>

using namespace std;

void dfs(int r, int c, int N, vector<vector<int>>& arr, vector<vector<int>>& visited, vector<int>& answer){
    if (N < 1){
        return;
    }
    int standard = arr[r][c];
    bool flag = false;
    for (int i = r; i < r+N; i++){
        for (int j = c; j < c+N; j++){
            if (standard != arr[i][j] || visited[i][j]){
                flag = true;
                break;
            }
        }
        if (flag){
            break;
        }
    }
    if (!flag){
        if (standard == 0) {
            answer[0]++;
        } else {
            answer[1]++;
        }
        for (int i = r; i < r+N; i++){
            for (int j = c; j < c+N; j++){
                visited[i][j] = 1;
            }
        }
    } else {
        int target = N/2;
        dfs(r, c, target, arr, visited, answer);
        dfs(r, c+target, target, arr, visited, answer);
        dfs(r+target, c, target, arr, visited, answer);
        dfs(r+target, c+target, target, arr, visited, answer);
    }
}

vector<int> solution(vector<vector<int>> arr) {
    vector<int> answer(2, 0);
    vector<vector<int>> visited(arr.size(), vector<int>(arr[0].size(), 0));
    int N = arr.size();
    
    dfs(0, 0, N, arr, visited, answer);
    
    return answer;
}