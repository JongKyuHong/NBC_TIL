#include <string>
#include <vector>
#include <algorithm>
#include <deque>

using namespace std;

vector<pair<int, int>> delta = {{0,1},{0,-1},{1,0},{-1,0}};
int N;
int M;

int bfs(int i, int j, vector<vector<int>>& vec, vector<vector<bool>>& visited){
    int sumV = vec[i][j];
    visited[i][j] = true;
    deque<pair<int, int>> dq;
    dq.push_back({i, j});
    while (!dq.empty()){
        pair<int, int> current = dq.front();
        // auto [r, c] = dq.front();
        dq.pop_front();
        int r = current.first;
        int c = current.second;
        
        for (int i = 0; i < 4; i++){
            int nr = delta[i].first + r;
            int nc = delta[i].second + c;
            
            if (nr >= N || nr < 0 || nc >= M || nc < 0 || visited[nr][nc] || vec[nr][nc] == -1){
                continue;
            }
            visited[nr][nc] = true;
            sumV += vec[nr][nc];
            dq.push_back({nr, nc});
        }
    }
    return sumV;
}

vector<int> solution(vector<string> maps) {
    vector<int> answer;
    
    N = maps.size();
    M = maps[0].length();
    vector<vector<int>> vec(N, vector<int>(M, -1));
    vector<vector<bool>> visited(N, vector<bool>(M, false));
    
    for (int i = 0; i < N; i++){
        for (int j = 0; j < M; j++){
            if (maps[i][j] != 'X'){
                vec[i][j] = maps[i][j] - '0';
            }
        }
    }
    
    for (int i = 0; i < N; i++){
        for (int j = 0; j < M; j++){
            if (!visited[i][j] && vec[i][j] != -1){
                answer.push_back(bfs(i, j, vec, visited));
            }
        }
    }
    
    if (!answer.empty()){
        sort(answer.begin(), answer.end());
        return answer;
    } else {
        return {-1};
    }
}