#include <iostream>
#include <algorithm>
#include <vector>
#include <string>

using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(NULL);

    int N;
    cin >> N;

    vector<string> vec;
    
    vector<int> v(N);
    for (int i = 0; i < N; i++) {
        cin >> v[i];
    }

    if (next_permutation(v.begin(), v.end())) {
        for (int x : v) {
            cout << x << ' ';
        }
        cout << '\n';
    } else {
        cout << -1 << '\n';
    }


    return 0;
}