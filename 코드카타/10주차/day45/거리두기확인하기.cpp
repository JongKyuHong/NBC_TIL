#include <string>
#include <vector>
#include <deque>

using namespace std;

bool bfs(vector<string>& place){
    vector<pair<int, int>> delta = {{1, 0}, {-1, 0}, {0, 1}, {0,-1}};
    
    
    for (int i = 0; i < 5; i++){
        for (int j = 0; j < 5; j++){
            if (place[i][j] == 'P'){
                vector<vector<int>> visited(5, vector<int>(5, -1));
                deque<vector<int>> dq;
                dq.push_back({i, j, 0});
                visited[i][j] = 0;
                while (!dq.empty()){
                    int r = dq.front()[0];
                    int c = dq.front()[1];
                    int dist = dq.front()[2];
                    dq.pop_front();
                    for (int k = 0; k < 4; k++){
                        int nr = r + delta[k].first;
                        int nc = c + delta[k].second;

                        if (nr >= 5 || nr < 0 || nc >= 5 || nc < 0 || visited[nr][nc] != -1 || place[nr][nc] == 'X'){
                            continue;
                        }
                        
                        if (place[nr][nc] == 'P'){
                            if (dist + 1 <= 2){
                                return false;
                            }
                        }
                        visited[nr][nc] = dist+1;
                        dq.push_back({nr, nc, dist+1});
                    }
                }
            }
        }
    }
    
    return true;
}

vector<int> solution(vector<vector<string>> places) {
    vector<int> answer;
    for (vector<string>& place : places){
        if (bfs(place)){
            answer.push_back(1);
        } else {
            answer.push_back(0);
        }
    }
    
    
    return answer;
}