// https://www.acmicpc.net/problem/24480
#include <string>
#include <vector>
#include <algorithm>
#include <iostream>

using namespace std;
int sequence = 1;

void dfs(const int N, const int R, vector<vector<int>> &adj, vector<int> &visited)
{
    visited[R] = sequence++;
    for (const int v : adj[R]){
        if (!visited[v])
        {
            dfs(N, v, adj, visited);
        }
    }
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(NULL);
    
    int N, M, R;
    cin >> N >> M >> R;
    
    vector<vector<int>> adj(N, vector<int>(0));
    
    for (int i = 0; i < M; i++)
    {
        int u, v;
        cin >> u >> v;
        adj[u-1].push_back(v-1);
        adj[v-1].push_back(u-1);
    }
    
    for (int i = 0; i < N; i++)
    {
        if (adj[i].size() > 1)
        {
            sort(adj[i].begin(), adj[i].end(), greater<int>());
        }
    }
    vector<int> visited(N, 0);
    dfs(N, R-1, adj, visited);
    for (int i = 0; i < N; i++)
    {
        cout << visited[i] << "\n";
    }
    
    return 0;
}