// https://www.acmicpc.net/problem/1774

#include <algorithm>
#include <iostream>
#include <vector>
#include <cmath>

using namespace std;

vector<int> parent(1001);

struct ver
{
    double cost;
    int start, end;
};

int find(int x)
{
    if (parent[x] == x) return x;
    return parent[x] = find(parent[x]);
}

bool union_find(int x, int y)
{
    x = find(x);
    y = find(y);
    if (x == y) return false;
    parent[x] = y;
    return true;
}

int main()
{
    ios_base::sync_with_stdio(false);
    cin.tie(0);

    int N, M;
    cin >> N >> M;
    
    for (int i = 1; i <= N; i++)
    {
        parent[i] = i;
    }
    
    vector<pair<int, int>> vec;
    for (int i = 0; i < N; i++)
    {
        int X, Y;
        cin >> X >> Y;
        vec.push_back({X, Y});
    }
    
    for (int i = 0; i < M; i++)
    {
        int X, Y;
        // 이미 연결된 통로
        cin >> X >> Y;
        union_find(X, Y);
    }
    
    vector<ver> vertex;
    
    for (int i = 0; i < N; i++)
    {
        for (int j = i+1; j < N; j++)
        {
            long long deltaX = (long long)vec[i].first - vec[j].first;
            long long deltaY = (long long)vec[i].second - vec[j].second;
            
            double dist = sqrt(deltaX * deltaX + deltaY  * deltaY);
            vertex.push_back({dist, i+1, j+1});
        }
    }
    
    sort(vertex.begin(), vertex.end(), [](const auto& a, const auto& b)
    {
        return a.cost < b.cost;
    });
    double total = 0;
    for (int i = 0; i < (int)vertex.size(); i++)
    {
        double cost = vertex[i].cost;
        int start = vertex[i].start;
        int end = vertex[i].end;
        
        if (union_find(start, end))
        {
            total += cost;
        }
    }
    cout << fixed;
    cout.precision(2);
    cout << total;
    return 0;
}