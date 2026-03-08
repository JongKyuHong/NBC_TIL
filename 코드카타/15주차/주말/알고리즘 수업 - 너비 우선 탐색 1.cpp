#include <queue>
#include <vector>
#include <algorithm>
#include <iostream>

using namespace std;

void bfs(int start, vector<vector<int>> &vec, vector<int> &visited)
{
    queue<int> q;
    q.push(start);
    
    int sequence = 1;
    visited[start] = sequence++;
    
    while (!q.empty())
    {
        int current = q.front();
        q.pop();
        if (!vec[current].empty())
        {
            for (int i = 0; i < vec[current].size(); i++)
            {
                if (visited[vec[current][i]] == 0)
                {
                    visited[vec[current][i]] = sequence++;
                    q.push(vec[current][i]);    
                }
            }    
        }
    }
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(NULL);
    
    int N, M, R;
    cin >> N >> M >> R;
    
    vector<vector<int>> vec(N);
    
    for (int i = 0; i < M; i++)
    {
        int u, v;
        cin >> u >> v;
        
        vec[u-1].push_back(v-1);
        vec[v-1].push_back(u-1);
    }
    
    for (int i = 0; i < N; i++)
    {
        sort(vec[i].begin(), vec[i].end());
    }
    
    vector<int> visited(N, 0);
    bfs(R-1, vec, visited);
    
    for (int i = 0; i < N; i++)
    {
        cout << visited[i] << "\n";
    }
    
    return 0;
}