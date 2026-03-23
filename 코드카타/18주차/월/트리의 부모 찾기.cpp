// https://www.acmicpc.net/problem/11725

#include <iostream>
#include <vector>
#include <queue>

using namespace std;

int parent[100001];
vector<vector<int>> vec(100001);
int N;

void bfs(int start)
{
    queue<int> q;
    vector<int> visited(N+1, 0);
    visited[start] = 1;
    q.push(start);
    while (!q.empty())
    {
        int node = q.front();
        q.pop();
        for (int i = 0; i < (int)vec[node].size(); i++)
        {
            int target = vec[node][i];
            if (visited[target] == 0)
            {
                visited[target] = 1;
                parent[target] = node;
                q.push(target);
            }
        }
    }
}

int main()
{
    ios_base::sync_with_stdio(false);
    cin.tie(0);

    
    cin >> N;
    
    for (int i = 0; i < N; i++)
    {
        parent[i] = 0;
    }
    
    for (int i = 0; i < N-1; i++)
    {
        int a, b;
        cin >> a >> b;
        vec[a].push_back(b);
        vec[b].push_back(a);
    }
    
    bfs(1);
    
    for (int i = 2; i <= N; i++)
    {
        cout << parent[i] << "\n";
    }
  
    return 0;
}