// https://www.acmicpc.net/problem/1197

#include <algorithm>
#include <iostream>
#include <vector>

using namespace std;

vector<int> parent(10001);

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

    int V, E;
    cin >> V >> E;
    
    for (int i = 1; i <= V; i++)
    {
        parent[i] = i;
    }
    
    vector<vector<int>> vec;
    
    for (int i = 0; i < E; i++)
    {
        int A, B, C;
        cin >> A >> B >> C;
        vec.push_back({C,A, B});
    }
    
    sort(vec.begin(), vec.end(), [](const auto& a, const auto& b)
    {
        return a[0] < b[0];    
    });
    
    int value = 0;
    
    for (int i = 0; i < E; i++)
    {
        int cost = vec[i][0];
        int start = vec[i][1];
        int end = vec[i][2];
        
        if (union_find(start, end))
        {
            value += cost;
        }
    }
    cout << value;
    
    return 0;
}