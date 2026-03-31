// https://www.acmicpc.net/problem/1766

#include <iostream>
#include <queue>
#include <vector>

using namespace std;

struct Compare
{
    bool operator()(const pair<int, int>& a, const pair<int, int>& b)
    {
        if (a.first != b.first) return a.first > b.first;
        return a.second > b.second;
    }
};

int main()
{
    ios_base::sync_with_stdio(false);
    cin.tie(0);
    
    int N, M;
    cin >> N >> M;
    
    vector<vector<int>> adj(N+1);
    vector<int> indegree(N+1);
    
    for (int i = 0; i < M; i++)
    {
        int A, B;
        cin >> A >> B;
        adj[A].push_back(B);
        indegree[B]++;
    }
    
    priority_queue<pair<int, int>, vector<pair<int, int>>, Compare> pq;
    for (int i = 1; i <= N; i++)
    {
        if (indegree[i] == 0)
        {
            pq.push({0, i});
        }
    }
    
    vector<int> result;
    while (!pq.empty())
    {
        int current = pq.top().second;
        pq.pop();
        result.push_back(current);
        for (int next : adj[current])
        {
            indegree[next]--;
            if (indegree[next] == 0)
            {
                pq.push({0, next});
            }
        }
    }
    for (int r : result)
    {
        cout << r << " ";
    }
    return 0;
}