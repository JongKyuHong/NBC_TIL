// https://www.acmicpc.net/problem/4948

#include <iostream>

using namespace std;

bool isPrime(int n)
{
    if (n <= 1) return false;
    for (int i = 2; i * i <= n; i++)
    {
        if (n % i == 0) return false;
    }
    return true;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(NULL);

    while (true)
    {
        int n;
        cin >> n;
        if (n == 0) break;
        int count = 0;
        for (int i = n+1; i <= 2*n; i++)
        {
            if (isPrime(i)) count++; 
        }
        cout << count << "\n";
    }
    
    return 0;
}