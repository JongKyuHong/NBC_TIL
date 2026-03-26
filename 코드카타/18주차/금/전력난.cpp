// https://www.acmicpc.net/problem/6497

#include <iostream>
#include <vector>
#include <queue>

using namespace std;

int main()
{
    ios_base::sync_with_stdio(false);
    cin.tie(0);

    int m, n;
    while (cin >> m >> n)
    {
        if (m == 0 && n == 0) break;
        int total = 0;
        
        // for (int i = 0; i < m; i++)
        // {
        //     visited[i] = 0;
        //     vec[i].clear();
        // }
        vector<int> visited(200001);
        vector<vector<pair<int, int>>> vec(200001);
        
        for (int i = 0; i < n; i++)
        {
            int x, y, z;
            cin >> x >> y >> z;
            vec[x].push_back({z, y});
            vec[y].push_back({z, x});
            total += z;
        }
        
        priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int,int>>> pq;
        pq.push({0, 0});
        int count = 0;
        while (!pq.empty())
        {
            int cost = pq.top().first;
            int node = pq.top().second;
            pq.pop();
            if (visited[node]) continue;
            if (count == m) break;
            total -= cost;
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
        cout << total << "\n";
    }
    
    return 0;
}