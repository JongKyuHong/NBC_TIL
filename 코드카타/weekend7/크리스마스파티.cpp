#include <iostream>
#include <vector>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(NULL);
    int N;
    cin >> N;
    int M;
    cin >> M;
    vector<int> score(N, 0);
    vector<int> targets;
    for (int i = 0; i < M; i++) {
        int inp = 0;
        cin >> inp;
        targets.push_back(inp);
    }

    for (int i = 0; i < M; i++) {
        int target = targets[i];
        int sumV = 0;
        for (int j = 0; j < N; j++) {
            int inp = 0;
            cin >> inp;
            // 타겟의 차례는 건너뛴다.
            if (target - 1 == j) {
                sumV++;
                continue;
            }
            if (target == inp) {
                score[j]++;
            } else {
                sumV++;
            }
        }
        score[target - 1] += sumV;
    }

    for (int s : score) {
        cout << s << "\n";
    }

    return 0;
}