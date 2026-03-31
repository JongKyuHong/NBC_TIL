// https://www.acmicpc.net/problem/2252

#include <iostream>
#include <queue>
#include <vector>

using namespace std;

int main()
{
    ios_base::sync_with_stdio(false);
    cin.tie(0);
    
    int N, M;
    cin >> N >> M;
    vector<vector<int>> adj(N+1);
    vector<int> indegree(N+1, 0);
    
    for (int i = 0; i < M; i++)
    {
        int A, B;
        cin >> A >> B;
        adj[A].push_back(B);
        indegree[B]++;
    }

    queue<int> q;
    for (int i = 1; i<=N; i++)
    {
        if (indegree[i] == 0)
        {
            q.push(i);
        }
    }
    vector<int> result;
    
    while (!q.empty())
    {
        int current = q.front();
        q.pop();
        result.push_back(current);
        
        for (int next : adj[current])
        {
            indegree[next]--;
            if (indegree[next] == 0)
            {
                q.push(next);
            }
        }
    }
    
    for (int r : result)
    {
        cout << r << " ";
    }
    
    return 0;
}