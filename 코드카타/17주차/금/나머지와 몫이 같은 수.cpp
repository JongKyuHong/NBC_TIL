// https://www.acmicpc.net/problem/1834

#include <iostream>

using namespace std;

int main()
{
    ios_base::sync_with_stdio(false);
    cin.tie(0);
    
    int N;
    cin >> N;
    
    // N으로 나누었을때 나머지와 몫이 같은애들
    
    long long result = 0;
    
    for (int i = 1; i < N; i++)
    {
        // 여기까지
        result += (long long)N*i + i;
    }
    
    cout << result;
    
    return 0;
}