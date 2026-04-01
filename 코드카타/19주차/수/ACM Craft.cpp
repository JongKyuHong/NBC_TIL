// https://www.acmicpc.net/problem/1005

#include <iostream>
#include <queue>
#include <vector>

using namespace std;

int main()
{
    ios_base::sync_with_stdio(false);
    cin.tie(0);
    
    int T;
    cin >> T;
    for (int t = 0; t < T; t++)
    {
        int N, K;
        cin >> N >> K;
        vector<int> D(N+1);
        for (int i = 1; i <= N; i++)
        {
            cin >> D[i];
        }
        
        vector<vector<int>> adj(N+1);
        vector<int> indegree(N+1);
        
        for (int i = 0; i < K; i++)
        {
            int X, Y;
            cin >> X >> Y;
            indegree[Y]++;
            adj[X].push_back(Y);
        }
        
        int W;
        cin >> W;
        // 중요한건 같은 차수의 건물이 지어질때 시간이 같이 흘러가야함
        vector<int> dp(N+1, 0);
        queue<int> q;
        for (int i = 1; i<=N;i++)
        {
            if (indegree[i] == 0)
            {
                q.push(i);
                dp[i] = D[i];
            }
        }
        
        while (!q.empty())
        {
            int current = q.front();
            q.pop();
            if (adj[current].empty()) continue;
            for (int next : adj[current])
            {
                dp[next] = max(dp[next], dp[current] + D[next]);
                indegree[next]--;
                if (indegree[next] == 0)
                {
                    q.push(next);    
                }
            }
        }
        cout << dp[W] << "\n";
    }
    
    return 0;
}