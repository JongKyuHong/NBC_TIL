// https://www.acmicpc.net/problem/2075

#include <iostream>
#include <queue>

using namespace std;

int main()
{
    ios_base::sync_with_stdio(false);
    cin.tie(0);

    int N;
    cin >> N;

    priority_queue<int> pq;
    int count = 0;
    for (int i= 0; i < N; i++)
    {
        for (int j = 0; j < N; j++)
        {
            int inp;
            cin >> inp;
            count++;
            pq.push(-inp);
            if (count == N+1)
            {
                count--;
                pq.pop();
            }
        }
    }

    cout << -pq.top();
    return 0;
}