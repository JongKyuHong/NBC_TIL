// https://www.acmicpc.net/problem/14621

#include <iostream>
#include <vector>
#include <queue>

using namespace std;

int main()
{
    ios_base::sync_with_stdio(false);
    cin.tie(0);

    int N, M;
    cin >> N >> M;
    
    vector<char> vec(N+1);
    for (int i = 1; i <= N; i++)
    {
        cin >> vec[i];
    }
    
    vector<vector<pair<int, int>>> vec2(N+1);
    for (int i = 0; i < M; i++)
    {
        int u, v, d;
        cin >> u >> v >> d;
        vec2[u].push_back({d, v});
        vec2[v].push_back({d, u});
    }
    
    vector<int> visited(N+1, 0);
    priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> pq;
    pq.push({0, 1});
    int total = 0;
    int count = 0;
    while (!pq.empty())
    {
        int cost = pq.top().first;
        int node = pq.top().second;
        pq.pop();
        if (visited[node]) continue;
        if (count == N) break;
        total += cost;
        count++;
        visited[node] = 1;
        if (!vec2[node].empty())
        {
            for (int i = 0; i < (int)vec2[node].size(); i++)
            {
                int newCost = vec2[node][i].first;
                int newNode = vec2[node][i].second;
                if (visited[newNode] == 0 && vec[node] != vec[newNode])
                {
                    pq.push({newCost, newNode});
                }
            }
        }
    }
    
    if (count == N)
    {
        cout << total;
    } else
    {
        cout << -1;
    }
    return 0;
}