// https://school.programmers.co.kr/learn/courses/30/lessons/1829

#include <vector>
#include <queue>
#include <iostream>

using namespace std;

const vector<pair<int, int>> delta = {{0,1}, {0,-1}, {1,0},{-1,0}};
int M;
int N;

int bfs(int x, int y, vector<vector<bool>>& visited, vector<vector<int>>& picture){
    visited[x][y] = true;
    queue<pair<int, int>> q;
    q.push({x, y});
    int cnt = 1;
    int standard = picture[x][y];
    
    while (!q.empty()){
        int r = q.front().first;
        int c = q.front().second;
        q.pop();
        for (int i = 0; i < 4; ++i){
            int nr = r + delta[i].first;
            int nc = c + delta[i].second;
            if (nr >= M || nr < 0 || nc >= N || nc < 0 || visited[nr][nc] || standard != picture[nr][nc]) continue;
            q.push({nr, nc});
            visited[nr][nc] = true;
            cnt++;
        }
    }
    return cnt;
}

vector<int> solution(int m, int n, vector<vector<int>> picture) {
    int number_of_area = 0;
    int max_size_of_one_area = 0;
    
    vector<vector<bool>> visited(m, vector<bool>(n, false));
    M = m;
    N = n;
    // bfs를 돌려서 영역의 크기를 반환하면 됨, bfs돌린 횟수도 저장
    for (int i = 0; i < m; ++i){
        for (int j = 0; j < n; ++j){
            if (!visited[i][j] && picture[i][j] != 0) {
                max_size_of_one_area = max(max_size_of_one_area, bfs(i, j, visited, picture));
                number_of_area++;
            }
        }
    }
    
    vector<int> answer(2);
    answer[0] = number_of_area;
    answer[1] = max_size_of_one_area;
    
        
    
    return answer;
}