#include <iostream>
#include <unordered_map>
#include <algorithm>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(NULL);
    int N;
    cin >> N;
    unordered_map<string, int> um;
    char prev = '\0';
    bool flag = false;
    char next = '\0';

    bool flag1 = false; // 물음표가 0번째 인덱스일 경우
    bool flag2 = false; // 물음표가 마지막 인덱스일 경우

    for (int i = 0; i < N; i++) {
        string inp;
        cin >> inp;
        um[inp]++;
        if (inp == "?") {
            if (i == 0) {
                flag1 = true;
            } else if (i == N - 1) {
                flag2 = true;
            }
            flag = true;
        } else {
            if (flag) {
                next = inp[0];
                flag = false;
            } else {
                if (next != '\0') {
                    continue;
                } else {
                    prev = inp[inp.length() - 1];
                }
            }
        }
    }

    int M;
    cin >> M;

    string answer = "";
    for (int i = 0; i < M; i++) {
        string inp;
        cin >> inp;
        if (N == 1) {
            answer = inp;
        }
        if (um.find(inp) == um.end()) {
            if (flag1) {
                if (inp[inp.length() - 1] == next) {
                    answer = inp;
                }
            } else if (flag2) {
                if (inp[0] == prev) {
                    answer = inp;
                }
            } else {
                if (inp[0] == prev && inp[inp.length() - 1] == next) {
                    answer = inp;
                }
            }
        }
    }
    cout << answer;
    return 0;
}