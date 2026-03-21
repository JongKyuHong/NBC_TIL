// https://www.acmicpc.net/problem/2581

#include <iostream>
#include <cmath>

using namespace std;

bool isPrime(int n)
{
    if (n < 2) return false;
    for (int i = 2; i * i <= n; i++)
    {
        if (n % i == 0)
        {
            return false;
        }
    }
    return true;
}

int main()
{
    ios_base::sync_with_stdio(false);
    cin.tie(0);

    int M, N;
    cin >> M;
    cin >> N;
    
    int minV = -1;
    int sumV = 0;
    for (int i = M; i <= N; i++)
    {
        if (isPrime(i))
        {
            if (minV == -1)
            {
                minV = i;
            }    
            sumV += i;
        }
    }
    if (minV == -1)
    {
        cout << -1;
    } else
    {
        cout << sumV << "\n" << minV;    
    }
    
    return 0;
}