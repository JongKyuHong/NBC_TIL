// https://www.acmicpc.net/problem/9372

#include <iostream>
#include <vector>
#include <queue>

using namespace std;

vector<pair<int, int>> delta = {{0,1 },{0,-1},{1,0},{-1,0}};

int main()
{
    ios_base::sync_with_stdio(false);
    cin.tie(0);
    
    int T;
    cin >> T;
    for (int t = 0; t < T; t++)
    {
        int N, M;
        cin >> N >> M;
        vector<vector<int>> vec(N+1);
        for (int i = 0; i < M; i++)
        {
            int a, b;
            cin >> a >> b;
            vec[a].push_back(b);
            vec[b].push_back(a);
        }
        
        vector<int> visited(N+1, 0);
        
        queue<int> q;
        q.push(1);
        visited[1] = 1;
        int total = 0;
        while (!q.empty())
        {
            int node = q.front();
            visited[node] = 1;
            q.pop();
            
            for (const int n : vec[node])
            {
                if (visited[n] == 0)
                {
                    visited[n] = 1;
                    total++;
                    q.push(n);
                }
            }
        }
        cout << total << "\n";
    }
    
    return 0;
}