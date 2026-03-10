// https://www.acmicpc.net/problem/11441

#include <iostream>
#include <vector>

using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(NULL);
    
    int N;
    cin >> N;
    
    vector<int> vec(N);
    
    for (int i = 0; i < N; i++) cin >> vec[i];
    
    int M;
    cin >> M;
    
    vector<long long> accumulation(N+1);
    accumulation[0] = 0;
    for (int i = 1; i <= N; i++)
    {
        accumulation[i] = accumulation[i-1] + vec[i-1];
    }
    
    for (int _ = 0; _ < M; _++)
    {
        int i, j;
        cin >> i >> j;
        cout << accumulation[j] - accumulation[i-1] << "\n";
    }
}