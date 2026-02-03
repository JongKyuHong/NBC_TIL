#include <string>
#include <vector>
#include <deque>

using namespace std;

int N, M;

int bfs(int i , int j, vector<string> maps){
    deque<pair<int, int>> dq;
    vector<vector<int>> delta = {{1, 0}, {-1, 0}, {0, 1}, {0,-1}};
    vector<vector<int>> visited(N, vector<int>(M, -1));
    dq.push_back({i, j});
    visited[i][j] = 0;
    
    // 우선 E와 L까지의 거리 재기
    int lenL = 0;
    int LR = 0;
    int LC = 0;
    
    while (!dq.empty()){
        auto [r, c] = dq.front();
        dq.pop_front();
        for (int k = 0; k < 4; k++){
            int nr = r + delta[k][0];
            int nc = c + delta[k][1];
            
            if (nr >= N || nc >= M || nc < 0 || nr < 0 || visited[nr][nc] != -1 || maps[nr][nc] == 'X'){
                continue;
            }
            
            visited[nr][nc] = visited[r][c] + 1;
            if (maps[nr][nc] == 'L'){
                lenL = visited[nr][nc];
                LR = nr;
                LC = nc;
            } else if (maps[nr][nc] == 'E'){
                
            }
            dq.push_back({nr, nc});
        }
    }

    if (lenL == 0){
        return 0;
    }
    
    vector<vector<int>> visited2(N, vector<int>(M, -1));
    
    dq.push_back({LR, LC});
    visited2[LR][LC] = 0;
    int minV = 0;
    while (!dq.empty()){
        auto [r, c] = dq.front();
        dq.pop_front();
        
        for (int k = 0; k < 4; k++){
            int nr = r + delta[k][0];
            int nc = c + delta[k][1];
            
            if (nr >= N || nc >= M || nc < 0 || nr < 0 || visited2[nr][nc] != -1 || maps[nr][nc] == 'X'){
                continue;
            }
            
            visited2[nr][nc] = visited2[r][c] + 1;
            if (maps[nr][nc] == 'E'){
                minV = visited2[nr][nc];
            }
            dq.push_back({nr, nc});
        }
    }
    if (minV == 0){
        return 0;
    }
    minV += lenL;
    return minV;
}

int solution(vector<string> maps) {
    int answer = 0;
    
    N = maps.size();
    M = maps[0].size();
    
    bool flag = false;
    for (int i = 0; i < N; i++){
        for (int j = 0; j < M; j++){
            if (maps[i][j] == 'S'){
                answer = bfs(i, j, maps);
                flag = true;
                break;
            }
        }
        if (flag){
            break;
        }
    }
    
    
    return answer == 0 ? -1 : answer;
}