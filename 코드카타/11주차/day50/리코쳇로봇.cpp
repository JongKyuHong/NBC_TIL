#include <string>
#include <vector>
#include <deque>
#include <iostream>

using namespace std;

int solution(vector<string> board) {
    int answer = -1;
    vector<pair<int, int>> delta = {{1, 0}, {-1, 0}, {0, 1}, {0, -1}};
    int N = board.size();
    int M = board[0].length();
    vector<vector<int>> visited(N, vector<int>(M, -1));
    deque<pair<int, int>> dq;

    for (int i = 0; i < N; i++){
        for (int j = 0; j < M; j++){
            if (board[i][j] == 'R'){
                dq.push_back({i, j});
                visited[i][j] = 0;
                while (!dq.empty()){
                    auto [r, c] = dq.front();
                    dq.pop_front();
                    for (int k = 0; k < 4; k++){
                        int nr = r;
                        int nc = c;
                        while (true){
                            nr += delta[k].first;
                            nc += delta[k].second;
                            if (nr >= 0 && nr < N && nc >= 0 && nc < M && board[nr][nc] != 'D'){
                                continue;
                            } else {
                                nr -= delta[k].first;
                                nc -= delta[k].second;
                                break;
                            }
                        }
                        
                        if (visited[nr][nc] == -1){
                            visited[nr][nc] = visited[r][c] + 1;
                            if (board[nr][nc] == 'G'){
                                return visited[nr][nc];
                            }
                            dq.push_back({nr, nc});    
                        }
                    }
                }
                
            }
        }
    }
    
    return answer;
}