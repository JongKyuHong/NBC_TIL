// https://www.acmicpc.net/problem/13565

#include <iostream>
#include <vector>
#include <queue>

using namespace std;

int M, N;

bool bfs(vector<vector<int>>& vec)
{
    vector<vector<int>> delta = {{0,1}, {0,-1},{1,0},{-1,0}};
    vector<vector<int>> visited(M, vector<int>(N, 0));
    queue<pair<int, int>> q;
    for (int i = 0; i < N; i++)
    {
        if (vec[0][i] == 0)
        {
            q.push({0, i});
            visited[0][i] = 1;
        }
    }
    
    while (!q.empty())
    {
        int r = q.front().first;
        int c = q.front().second;
        if (r == M-1) return true;
        q.pop();
        
        for (int i = 0; i < 4; i++)
        {
            int nr = r + delta[i][0];
            int nc = c + delta[i][1];
            
            if (nr >= M || nc >= N || nr < 0 || nc < 0 || visited[nr][nc] || vec[nr][nc] == 1) continue;
            visited[nr][nc] = 1;
            q.push({nr, nc});
        }
    }
    return false;
}

int main()
{
    ios_base::sync_with_stdio(false);
    cin.tie(0);

    cin >> M >> N;

    vector<vector<int>> vec(M, vector<int>(N));
    
    for (int i = 0; i < M; i++)
    {
        string inp;
        cin >> inp;
        for (int j = 0; j < N; j++)
        {
            vec[i][j] = inp[j] - '0';
        }
    }
    
    // 위쪽은 모두 아웃사이드
    if (bfs(vec))
    {
        cout << "YES";
    } else
    {
        cout << "NO";
    }
    
    return 0;
}