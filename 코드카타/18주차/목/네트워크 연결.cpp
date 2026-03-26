// https://www.acmicpc.net/problem/1922

#include <iostream>
#include <queue>
#include <vector>

using namespace std;

int main()
{
    ios_base::sync_with_stdio(false);
    cin.tie(0);

    int N;
    cin >> N;
    int M;
    cin >> M;
    
    vector<vector<pair<int, int>>> vec(N+1);
    for (int i = 0; i < M; i++)
    {
        int a, b, c;
        cin >> a >> b >> c;
        vec[a].push_back({c, b});
        vec[b].push_back({c, a});
    }
    
    priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> pq;
    vector<int> visited(N+1);
    
    pq.push({0, 1});
    int total = 0;
    int count = 0;
    
    while (!pq.empty())
    {
        int cost = pq.top().first;
        int node = pq.top().second;
        pq.pop();
        
        if (count == N) break;
        if (visited[node]) continue;
        
        total += cost;
        count++;
        visited[node] = 1;
        
        for (int i = 0; i < (int)vec[node].size(); i++)
        {
            int newCost = vec[node][i].first;
            int newNode = vec[node][i].second;
            if (visited[newNode] == 0)
            {
                pq.push({newCost, newNode});
            }
        }
    }
    
    cout << total;
    
    return 0;
}