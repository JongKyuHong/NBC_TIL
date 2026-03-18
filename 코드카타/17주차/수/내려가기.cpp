// https://www.acmicpc.net/problem/2096

#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(NULL);
    
    int N;
    cin >> N;
    
    long long prevMinDp[3], prevMaxDp[3];
    long long currMinDp[3], currMaxDp[3];

    cin >> prevMaxDp[0] >> prevMaxDp[1] >> prevMaxDp[2];
    prevMinDp[0] = prevMaxDp[0]; prevMinDp[1] = prevMaxDp[1]; prevMinDp[2] = prevMaxDp[2];
    
    for (int i = 1; i < N; i++)
    {
        long long a, b, c;
        cin >> a >> b >> c;
        
        currMaxDp[0] = max(prevMaxDp[0], prevMaxDp[1]) + a;
        currMaxDp[1] = max({prevMaxDp[0], prevMaxDp[1], prevMaxDp[2]}) + b;
        currMaxDp[2] = max(prevMaxDp[1], prevMaxDp[2]) + c;
        
        currMinDp[0] = min(prevMinDp[0], prevMinDp[1]) + a;
        currMinDp[1] = min({prevMinDp[0], prevMinDp[1], prevMinDp[2]}) + b;
        currMinDp[2] = min(prevMinDp[1], prevMinDp[2]) + c;
        
        for (int j = 0; j < 3; j++)
        {
            prevMaxDp[j] = currMaxDp[j];
            prevMinDp[j] = currMinDp[j];
        }
    }

    long long maxV = max({prevMaxDp[0], prevMaxDp[1], prevMaxDp[2]});
    long long minV = min({prevMinDp[0], prevMinDp[1], prevMinDp[2]});
    
    cout << maxV << " " << minV;
    return 0;
}