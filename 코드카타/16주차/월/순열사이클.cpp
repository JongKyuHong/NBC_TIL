// https://www.acmicpc.net/problem/10451

#include <iostream>
#include <vector>

using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(NULL);
    
    int T = 0;
    cin >> T;
    
    for (int t = 0; t < T; t++)
    {
        int N = 0;
        cin >> N;
        vector<int> vec(N);
        for (int i = 0; i < N; i++)
        {
            int tmp = 0;
            cin >> tmp;
            // 0-index 사용
            vec[i] = tmp-1;
        }
        
        vector<int> visited(N, 0);
        int cycleCount = 0;
        for (int i = 0; i < N; i++)
        {
            if (visited[i] == 0)
            {
                int current = i;
                while (visited[current] == 0)
                {
                    visited[current] = 1;
                    current = vec[current];
                }
                cycleCount++;
            }
        }
        cout << cycleCount << "\n";
    }
    
    return 0;
}