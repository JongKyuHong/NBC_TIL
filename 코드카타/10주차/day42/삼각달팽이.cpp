#include <string>
#include <vector>

using namespace std;

vector<int> solution(int n) {
    vector<int> answer;
    int current = 1;
    vector<vector<int>> vec(n, vector<int>(n,0));

    int dr = 0;
    int dc = 0;
    vector<vector<int>> delta = {{1, 0}, {0,1}, {-1,-1}};
    int dir = 0;
    
    int target = 0;
    for (int i = 1; i <= n; i++){
        target += i;        
    }
    
    vec[dr][dc] = current++;
    
    while (current <= target){
        int nr = dr + delta[dir][0];
        int nc = dc + delta[dir][1];

        if (nr >= n || nc >= n || nr < 0 || nc < 0 || vec[nr][nc] != 0 || nr < nc){
            dir = (dir+1) % 3;
            continue;
        }
        vec[nr][nc] = current++;

        dr = nr;
        dc = nc;
    }
    
    for (int i = 0; i < n; i++){
        for (int j = 0; j <= i; j++){
            answer.push_back(vec[i][j]);
        }
    }
    
    return answer;
}