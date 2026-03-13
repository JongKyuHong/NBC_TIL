// https://www.acmicpc.net/problem/24511

#include <iostream>
#include <vector>
#include <queue>

using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(NULL);

    int N;
    cin >> N;
    
    vector<long long> A(N);
    for (int i = 0; i < N; i++)
    {
        cin >> A[i];
    }
    vector<long long> B(N);
    for (int i = 0; i < N; i++)
    {
        cin >> B[i];
    }
    
    // 0의 개수가 몇개인지 세고 걔네값만 따로 저장
    queue<long long> vec;
    for (int i = N-1; i >= 0; i--)
    {
        if (A[i] == 0)
        {
            vec.push(B[i]);
        }
    }
    
    int M;
    cin >> M;
    
    vector<long long> C(M);
    for (int i = 0; i < M; i++)
    {
        cin >> C[i];
    }
    
    for (int i = 0; i < M; i++)
    {
        // 이번 회차
        long long current = C[i];
        
        vec.push(current);
        cout << vec.front() << " ";
        vec.pop();
    }
    
    return 0;
}