// https://www.acmicpc.net/problem/13699

#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

int main()
{
    ios_base::sync_with_stdio(false);
    cin.tie(0);

    int n;
    cin >> n;
    
    vector<long long> dp(36, 0);
    dp[0] = 1;
    for (int i = 1; i <= 35; i++)
    {
        for (int j = 0; j <= i-1; j++)
        {
            dp[i] += dp[j]*dp[i-j-1];    
        }
    }

    cout << dp[n];
    return 0;
}