// https://school.programmers.co.kr/learn/courses/30/lessons/1844?language=cpp

#include <vector>
#include <queue>
#include <iostream>

using namespace std;

int N, M;
vector<pair<int, int>> delta = {{0, 1}, {0,-1}, {1,0}, {-1,0}};

int bfs(int i, int j, vector<vector<int>>& maps, vector<vector<bool>>& visited){
    queue<vector<int>> q;
    visited[i][j] = true;
    int distance = 1;
    q.push({i, j, distance});
    while(!q.empty()){
        int r = q.front()[0];
        int c = q.front()[1];
        int dist = q.front()[2];
        if (r == N-1 && c == M-1){
            return dist;
        }
        q.pop();
        for (int i = 0; i < 4; ++i){
            int nr = r + delta[i].first;
            int nc = c + delta[i].second;
            if (nr >= N || nr < 0 || nc >= M || nc < 0 || visited[nr][nc] || maps[nr][nc] == 0){
                continue;
            }
            visited[nr][nc] = true;
            q.push({nr, nc, dist+1});
        }
    }
    
    return distance;
}

int solution(vector<vector<int>> maps)
{
    int answer = 0;
    N = maps.size();
    M = maps[0].size();
    if (N == 1 && M == 1){
        return 1;
    }
    
    // 방문체크를 해주는 2차원벡터가 필요
    vector<vector<bool>> visited(N, vector<bool>(M, false));
    // 사용자의 위치 0, 0
    answer = bfs(0, 0, maps, visited);
    
    return answer == 1 ? -1 : answer;
}