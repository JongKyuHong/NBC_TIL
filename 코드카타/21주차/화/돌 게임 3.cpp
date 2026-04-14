// https://www.acmicpc.net/problem/9657

#include <iostream>
#include <vector>

using namespace std;

int main()
{
    ios_base::sync_with_stdio(false);
    cin.tie(0);
    
    int N;
    cin >> N;

    vector<int> dp(1001, 0);
    
    dp[1] = 1;
    dp[3] = 1;
    dp[4] = 1;
    
    for (int i = 5; i <= N; i++)
    {
        if (dp[i-1] == 0 || dp[i-3] == 0 || dp[i-4] == 0) dp[i] = 1; 
    }
    
    if (dp[N] == 1)
    {
        cout << "SK";
    } else
    {
        cout << "CY";
    }


    return 0;
}