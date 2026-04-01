// https://www.acmicpc.net/problem/1516

#include <iostream>
#include <queue>
#include <vector>
#include <string>

using namespace std;
// 2623다음문제
int main()
{
    ios_base::sync_with_stdio(false);
    cin.tie(0);
    
    int N;
    cin >> N;
    vector<vector<int>> adj(N+1);
    vector<int> RequiredTime(N+1);
    vector<int> indegree(N+1, 0);
    for (int i = 0; i < N; i++)
    {
        cin >> RequiredTime[i+1];
        while (true)
        {
            int inp;
            cin >> inp;
            if (inp == -1)
            {
                break;
            }
            indegree[i+1]++;
            adj[inp].push_back(i+1);
        }
    }
    
    queue<int> q;
    vector<int> dp(N+1, 0);
    
    for (int i = 1; i <= N; i++)
    {
        if (indegree[i] == 0)
        {
            q.push(i);
            dp[i] = RequiredTime[i];
        }
    }
    
    while (!q.empty())
    {
        int current = q.front();
        q.pop();
        if (adj[current].empty()) continue;
        for (int next : adj[current])
        {
            indegree[next]--;
            dp[next] = max(dp[next], dp[current] + RequiredTime[next]);
            if (indegree[next] == 0)
            {
                q.push(next);
            }
        }
    }
    
    for (int i = 1; i <= N; i++)
    {
        cout << dp[i] << "\n";
    }
    
    return 0;
}