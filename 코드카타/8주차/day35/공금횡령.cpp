#include <string>
#include <vector>
#include <iostream>
#include <unordered_map>

using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(NULL);

    int answer = 0;
    unordered_map<string, int> um;
    int N, M;
    cin >> N >> M;
    for (int i = 0; i < N; i++) {
        string a;
        int b;
        cin >> a >> b;
        um[a] = b;
    }

    for (int i = 0; i < M; i++) {
        string a;
        int b;
        cin >> a >> b;

        if ((um[a] * 105) / 100 < b) {
            answer++;
        }
    }
    cout << answer;
    return 0;
}
