#include <iostream>

using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(NULL);

    int N, M;

    cin >> N >> M;

    if (N*100 < M) {
        cout << "No";
    } else {
        cout << "Yes";
    }

    return 0;
}