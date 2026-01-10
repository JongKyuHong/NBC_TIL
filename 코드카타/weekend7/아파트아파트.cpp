#include <iostream>
#include <algorithm>
#include <vector>
#include <unordered_map>

using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(NULL);
    int N = 0;
    int M = 0;
    cin >> N >> M;
    vector<int> vec;
    unordered_map<int, int> um;

    for (int i = 0; i < M; i++) {
        int H1, H2;
        cin >> H1 >> H2;
        um[H1] = i+1;
        um[H2] = i+1;
        vec.push_back(H1);
        vec.push_back(H2);
    }

    sort(vec.begin(), vec.end());

    for (int i = 0; i < N; i++) {
        int target = vec[0];
        vec.erase(vec.begin());
        vec.push_back(target);
    }
    int answer = um[vec[vec.size() - 1]];
    cout << answer;

    return 0;
}