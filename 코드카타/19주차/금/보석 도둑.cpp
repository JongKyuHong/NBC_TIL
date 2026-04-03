// https://www.acmicpc.net/problem/1202

#include <algorithm>
#include <iostream>
#include <vector>
#include <queue>

using namespace std;


int main()
{
    ios_base::sync_with_stdio(false);
    cin.tie(0);
    
    int N, K;
    cin >> N >> K;
    
    vector<pair<int, int>> jewelry;
    for (int i = 0; i < N; i++)
    {
        int M, V;
        cin >> M >> V;
        jewelry.push_back({M, V});
    }
    sort(jewelry.begin(), jewelry.end(), [](pair<int, int> a, pair<int, int> b)
    {
        return a.first < b.first;
    });
    vector<long long> knapsack;
    for (int i = 0; i < K; i++)
    {
        long long C;
        cin >> C;
        knapsack.push_back(C);
    }
    sort(knapsack.begin(), knapsack.end());
    
    priority_queue<long long, vector<long long>> pq;
    
    int j = 0;
    long long total = 0;
    for (int i = 0; i < K; i++)
    {
        while (j < N && jewelry[j].first <= knapsack[i])
        {
            pq.push(jewelry[j].second);
            j++;
        }
        
        if (!pq.empty())
        {
            total += pq.top();
            pq.pop();
        }
    }
    
    cout << total;
    
    return 0;
}