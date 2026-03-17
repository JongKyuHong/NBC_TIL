// https://www.acmicpc.net/problem/11060

#include <iostream>
#include <vector>
#include <climits>

using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(NULL);

    int N;
    cin >> N;
    vector<int> A(N);
    for (int i = 0; i < N; i++)
    {
        cin >> A[i];
    }
    
    // 가장 왼쪽에서 시작
    int current = 0;
    int count = 0;
    // A[current] 범위안에 어디든 갈 수 있다.
    vector<int> dp(N, INT_MAX);
    dp[0] = 0;
    
    for (int i = 0; i < N; i++)
    {
        if (dp[i] == INT_MAX) continue;
        for (int j = 1; j <= A[i]; j++)
        {
            if (i+j >= N) break;
            dp[i+j] = min(dp[i+j], dp[i] + 1);
        }
    }
    cout << (dp[N-1] == INT_MAX ? -1 : dp[N-1]);
    
    return 0;
}