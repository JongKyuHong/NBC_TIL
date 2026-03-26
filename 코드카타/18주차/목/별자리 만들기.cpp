// https://www.acmicpc.net/problem/4386

#include <algorithm>
#include <iostream>
#include <vector>
#include <cmath>

using namespace std;

vector<int> parent(101);
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

    int n;
    cin >> n;
    
    vector<pair<double, double>> vec;
    for (int i = 1; i <= n; i++)
    {
        parent[i] = i;
    }
    
    for (int i = 0; i < n; i++)
    {
        double x, y;
        cin >> x >> y;
        vec.push_back({x, y});
    }
    
    vector<ver> vertex;
    for (int i = 0; i < n; i++)
    {
        for (int j = i+1; j < n; j++)
        {
            double deltaX = vec[i].first - vec[j].first;
            double deltaY = vec[i].second - vec[j].second;
            double dist = sqrt(deltaX * deltaX + deltaY * deltaY);
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
            total+=cost;
        }
    }
    
    cout << fixed;
    cout.precision(2);
    cout << total;
    
    return 0;
}