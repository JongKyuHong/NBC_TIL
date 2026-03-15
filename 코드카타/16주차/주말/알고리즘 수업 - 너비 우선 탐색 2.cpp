// https://www.acmicpc.net/problem/24445

#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>

using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(NULL);
    int N, M, R;
    cin >> N >> M >> R;
    vector<vector<int>> graph(N);
    for (int i = 0; i < M; i++)
    {
        int u, v;
        cin >> u >> v;
        graph[u-1].push_back(v-1);
        graph[v-1].push_back(u-1);
    }
    
    queue<int> q;
    q.push(R-1);
    vector<int> visited(N, 0);
    int order = 1;
    visited[R-1] = order++;
    while (!q.empty())
    {
        int current = q.front();
        
        q.pop();
        if (!graph[current].empty())
        {
            sort(graph[current].begin(), graph[current].end(), greater<int>());
            for (int i = 0; i < graph[current].size(); i++)
            {
                if (!visited[graph[current][i]])
                {
                    visited[graph[current][i]] = order++;
                    q.push(graph[current][i]);    
                }
            }    
        }
    }
    
    for (int i = 0; i < N; i++)
    {
        cout << visited[i] << "\n";
    }
    
    return 0;
}