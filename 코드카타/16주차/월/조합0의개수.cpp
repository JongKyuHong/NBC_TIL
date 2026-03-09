// https://www.acmicpc.net/problem/2004

#include <cmath>
#include <algorithm>
#include <iostream>

using namespace std;

long long calculate(long long targetNumber, int p)
{
    long long count = 0;
    while (targetNumber >= p)
    {
        count += targetNumber / p;
        targetNumber /= p;
    }
    return count;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(NULL);
    
    long long n, m;
    cin >> n >> m;
    
    if (m == 0)
    {
        cout << 0;
        return 0;
    }
    
    long long answer = 0;
    
    // 조합은 n! / m!(n-m)!이기도 하다.
    long long countTwo = calculate(n, 2) - calculate(m, 2) - calculate(n-m, 2);
    long long countFive = calculate(n, 5) - calculate(m, 5) - calculate(n-m, 5);
    
    cout << min(countTwo, countFive);
    
    // 각각 2와 5의 개수를 구해
    //
    // n의 2 x m의 5 
    // n의 5 x m의 2
    // 이게 총 0의 개수
    // 각각의 최소값을 answer에 더해준다.
    
    return 0;
}