// https://www.acmicpc.net/problem/4485

#include <iostream>
#include <vector>
#include <queue>

using namespace std;

vector<vector<int>> delta = {{0, 1}, {0, -1}, {1, 0}, {-1, 0}};

int dijkstra(int row, int col, vector<vector<int>>& vec, vector<vector<int>>& visited)
{
    visited[row][col] = vec[0][0];
    priority_queue<pair<int, pair<int, int>>> pq;
    int N = (int)vec.size();
    pq.push({-vec[0][0], {row, col}});
    while (!pq.empty())
    {
        int dist = -pq.top().first;
        int r = pq.top().second.first;
        int c = pq.top().second.second;
        pq.pop();
        
        if (visited[r][c] < dist) continue;
        
        for (int i = 0; i < 4; i++)
        {
            int nr = r + delta[i][0];
            int nc = c + delta[i][1];
            
            if (nr < 0 || nc < 0 || nr >= N || nc >= N) continue;
            
            if (visited[nr][nc] > dist + vec[nr][nc])
            {
                visited[nr][nc] = dist + vec[nr][nc];
                pq.push({-visited[nr][nc], {nr, nc}});
            }
        }
    }
    
    return visited[N-1][N-1];
}

int main()
{
    ios_base::sync_with_stdio(false);
    cin.tie(0);

    int N;
    int testcase = 1;
    while (cin >> N)
    {
        if (N == 0) break;
        vector<vector<int>> vec(N, vector<int>(N));
        vector<vector<int>> visited(N, vector<int>(N, 1e9));

        for (int i = 0; i < N; i++)
        {
            for (int j = 0; j < N; j++)
            {
                cin >> vec[i][j];
            }
        }
        
        int result = dijkstra(0, 0, vec, visited);
        cout << "Problem " << testcase++ << ": " << result << "\n";
    }
    
    return 0;
}