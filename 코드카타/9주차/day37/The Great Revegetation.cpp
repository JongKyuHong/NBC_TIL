#include <iostream>
#include <vector>

using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(NULL);

    int N, M;
    cin >> N >> M;
    
    vector<int> vec[101];
    int result[101] = { 0, };

    for (int i = 0; i < M; i++) {
        int u, v;
        cin >> u >> v;
        vec[u].push_back(v);
        vec[v].push_back(u);
    }

    for (int i = 1; i <= N; i++) {
        bool used[4] = { false, };

        for (int neighbor : vec[i]) {
            if (result[neighbor] != 0) {
                used[result[neighbor]-1] = true;
            }
        }

        for (int j = 0; j < 4; j++) {
            if (!used[j]) {
                result[i] = j+1;
                break;
            }
        }
    }

    for (int i = 1; i <= N; i++) {
        cout << result[i];
    }
    return 0;
}