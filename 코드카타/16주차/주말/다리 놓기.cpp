// https://www.acmicpc.net/problem/1010

#include <iostream>

using namespace std;

int dp[30][30];

int comb(int n, int r)
{
    if (n == r || r == 0) return dp[n][r] = 1;
    if (dp[n][r] > 0) return dp[n][r];
    return dp[n][r] = comb(n-1, r-1) + comb(n-1, r);
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(NULL);
    
    int T;
    cin >> T;
    long long answer = 1;
    
    for (int i = 0; i < T; i++)
    {
        int N, M;
        cin >> N >> M;
        
        cout << comb(M, N) << "\n";
    }

    return 0;
}