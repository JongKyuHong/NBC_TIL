#include <iostream>

using namespace std;

bool isPrime(long long n)
{
    if (n <= 1) return false;
    
    for (long long i = 2; i * i <= n; i++) {
        if (n % i == 0) return false;
    }
    
    return true;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(NULL);
    
    int T;
    cin >> T;

    for (int _ = 0; _ < T; _++)
    {
        long long n;
        cin >> n;
        long long answer = 0;
        while (true)
        {
            if (isPrime(n))
            {
                cout << n << "\n";
                break;
            }
            n++;
        }
    }
    
    
}