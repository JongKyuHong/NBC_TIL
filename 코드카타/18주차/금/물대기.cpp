// https://www.acmicpc.net/problem/1368

#include <iostream>
#include <vector>
#include <queue>

using namespace std;

int main()
{
    ios_base::sync_with_stdio(false);
    cin.tie(0);
    
    int N;
    cin >> N;
    
    // 직접 우물파는 비용
    int minIndex = -1;
    int minValue = 100001;
    vector<int> costs; 
    for (int i = 0; i < N; i++)
    {
        int W;
        cin >> W;
        costs.push_back(W);
        if (minValue > W)
        {
            minValue = W;
            minIndex = i;
        }
    }
    
    vector<vector<int>> vec(N, vector<int>(N));
    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < N; j++)
        {
            cin >> vec[i][j];
        }
    }
    
    priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> pq;
    vector<int> visited(N, 0);
    for (int i= 0; i < N; i++)
    {
        pq.push({costs[i], i});
    }
    int total = 0;
    int count = 0;
    while (!pq.empty())
    {
        int cost = pq.top().first;
        int node = pq.top().second;
        pq.pop();
        
        if (visited[node]) continue;
        total += cost;
        // cout << "노드순서 : " << node << ":" << cost << ":" << total << "\n";
        count++;
        visited[node] = 1;
        for (int i = 0; i < N; i++)
        {
            if (i == node) continue;
            // node에서 i로 가는 비용
            int newCost = vec[node][i];
            if (visited[i] == 0)
            {
                pq.push({newCost, i});
            }
        }
    }
    cout << total;
    
    return 0;
}