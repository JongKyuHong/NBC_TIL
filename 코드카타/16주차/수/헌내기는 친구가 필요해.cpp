// https://www.acmicpc.net/problem/21736

#include <iostream>
#include <vector>
#include <queue>

using namespace std;

int bfs(int startRow, int startCol, vector<vector<char>> &vec, vector<vector<bool>> &visited)
{
    vector<pair<int, int>> delta = {{0,1}, {0,-1}, {1,0}, {-1,0}};
    queue<pair<int, int>> q;
    q.push({startRow, startCol});
    visited[startRow][startCol] = true;
    int N = vec.size();
    int M = vec[0].size();
    int count = 0;
    while (!q.empty())
    {
        int r = q.front().first;
        int c = q.front().second;
        q.pop();
        for (int i = 0; i < 4; i++)
        {
            int nr = r + delta[i].first;
            int nc = c + delta[i].second;
            
            if (nr >= N || nc >= M || nr < 0 || nc < 0 || vec[nr][nc] == 'X' || visited[nr][nc])
            {
                continue;
            }
            visited[nr][nc] = true;
            if (vec[nr][nc] == 'P')
            {
                count++;
            }
            q.push({nr, nc});
        }
    }
    return count;
}


int main() {
    ios::sync_with_stdio(false);
    cin.tie(NULL);

    int N, M;
    cin >> N >> M;
    
    vector<vector<char>> vec(N, vector<char>(M));
    vector<vector<bool>> visited(N, vector<bool>(M));
    
    int startRow = 0;
    int startCol = 0;
    
    for (int i = 0; i < N; i++)
    {
        string tmp;
        cin >> tmp;
        for (int j = 0; j < M; j++)
        {
            if (tmp[j] == 'I')
            {
                startRow = i;
                startCol = j;
            }
            vec[i][j] = tmp[j]; 
        }
    }
    int answer = bfs(startRow, startCol, vec, visited);
    if (answer == 0)
    {
        cout << "TT";
    } else
    {
        cout << answer;    
    }
    return 0;
}