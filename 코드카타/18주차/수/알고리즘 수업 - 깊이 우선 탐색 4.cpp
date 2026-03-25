// https://www.acmicpc.net/problem/24482

#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

void dfs(int start, vector<vector<int>>& vec, vector<int>& visited, int depth)
{
    visited[start] = depth;
    sort(vec[start].begin(), vec[start].end(), greater<int>());
    for (const int& node : vec[start])
    {
        if (visited[node] == -1)
        {
            dfs(node, vec, visited, depth+1);
        }
    }
}

int main()
{
    ios_base::sync_with_stdio(false);
    cin.tie(0);

    int N, M, R;
    cin >> N >> M >> R;
    
    vector<vector<int>> vec(N);
    vector<int> visited(N, -1);
    
    for (int i = 0; i < M; i++)
    {
        int u, v;
        cin >> u >> v;
        u--;
        v--;
        vec[u].push_back(v);
        vec[v].push_back(u);
    }
    
    dfs(R-1,vec, visited, 0);
    
    for (const int& node : visited)
    {
        cout << node << "\n";
    }
    
    return 0;
}