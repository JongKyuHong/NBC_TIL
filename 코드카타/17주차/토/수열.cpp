// https://www.acmicpc.net/problem/2559

#include <algorithm>
#include <iostream>
#include <queue>

using namespace std;


int main()
{
    ios_base::sync_with_stdio(false);
    cin.tie(0);

    int N, K;
    cin >> N >> K;
    queue<int> q;
    
    int sumV = 0;
    int count = 0;
    int maxV = -1e9;
    
    for (int i = 0; i < N; i++)
    {
        int inp;
        cin >> inp;
        if (count < K)
        {
            q.push(inp);
            sumV += inp;
            count++;
            if (count == K)
            {
                maxV = max(maxV, sumV);
                count--;
                sumV -= q.front();
                q.pop();
            }
        }
    }
    
    cout << maxV;
    return 0;
}