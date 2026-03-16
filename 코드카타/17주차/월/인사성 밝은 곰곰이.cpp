// https://www.acmicpc.net/problem/25192

#include <iostream>
#include <set>

using namespace std;


int main() {
    ios::sync_with_stdio(false);
    cin.tie(NULL);

    int N;
    cin >> N;
    int answer = 0;
    set<string> S;
    for (int i = 0; i < N; i++)
    {
        string Inp;
        cin >> Inp;
        if (Inp == "ENTER")
        {
            // 리셋
            answer += (int)S.size();
            S.clear();
        } else
        {
            S.insert(Inp);
        }
    }
    answer += (int)S.size();
    cout << answer;
    return 0;
}