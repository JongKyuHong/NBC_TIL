// https://www.acmicpc.net/problem/1539

#include <iostream>
#include <vector>
#include <set>
#include <algorithm>

using namespace std;

int main()
{
    ios_base::sync_with_stdio(false);
    cin.tie(0);
    
    int N;
    cin >> N;
    vector<int> height(N+1, 0);
    set<int> s;
    long long total = 0;
    
    for (int i = 0; i < N; i++)
    {
        int val;
        cin >> val;
        
        auto it = s.lower_bound(val);
        
        int right_height = 0;
        int left_height = 0;
        if (it != s.end())
        {
            right_height = height[*it];
        }
        
        if (it != s.begin())
        {
            left_height = height[*prev(it)];
        }
        
        int value = max(left_height, right_height) + 1; 
        total += value;
        s.insert(val);
        height[val] = value;
    }
    cout << total;
    
    return 0;
}