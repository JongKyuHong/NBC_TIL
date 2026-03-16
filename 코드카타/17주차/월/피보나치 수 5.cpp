// https://www.acmicpc.net/problem/10870

#include <iostream>

using namespace std;

int dp[21];

int fibonacci(int n)
{
    if (n == 0 || n == 1) return n;
    if (dp[n] != 0) return dp[n];
    return dp[n] = fibonacci(n - 1) + fibonacci(n - 2);
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(NULL);

    int n;
    cin >> n;
    cout << fibonacci(n);
    
    return 0;
}