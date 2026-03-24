// https://www.acmicpc.net/problem/11657

#include <iostream>
#include <vector>

using namespace std;

struct Edge
{
    int from, to, cost;
};

const int INF = 1e9;

int main()
{
    ios_base::sync_with_stdio(false);
    cin.tie(0);
    
    int N, M;
    cin >> N >> M;
    
    vector<Edge> vec;
    
    for (int i = 0; i < M; i++)
    {
        int A, B, C;
        cin >> A >> B >> C;
        vec.push_back({A, B, C});
    }
    bool flag = false;
    vector<long long> dist(N+1, INF);
    dist[1] = 0;
    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < M; j++)
        {
            int from = vec[j].from;
            int to = vec[j].to;
            int cost = vec[j].cost;
            
            if (dist[from] != INF && dist[to] > dist[from] + cost)
            {
                dist[to] = dist[from] + cost;
                if (i == N-1)
                {
                    flag = true;
                }
            }
        }
    }
    
    if (flag)
    {
        cout << -1 << "\n";
    } else
    {
        for (int i = 2; i <= N; i++)
        {
            if (dist[i] == INF) cout << -1 << "\n";
            else cout << dist[i] << "\n";
        }
    }
    
    return 0;
}