#include <iostream>
#include <vector>
#include <algorithm>
#include <string>
#include <utility>

using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(NULL);

    int N;
    cin >> N;

    vector<pair<int, string>> vec(N);

    for (int i = 0; i < N; i++) {
        cin >> vec[i].first >> vec[i].second;
    }

    stable_sort(vec.begin(), vec.end(), [](const pair<int, string>& a, const pair<int, string>& b) {
        return a.first < b.first;
        });

    // stable_sort(vec.begin(), vec.end());

    for (pair<int, string>& v : vec) {
        cout << v.first << " " << v.second << "\n";
    }
    
    return 0;
}
