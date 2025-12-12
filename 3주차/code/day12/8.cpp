#include <iostream>
#include <vector>
#include <algorithm>
#include <string>
#include <map>

using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(NULL);

    int N;
    cin >> N;

    vector<int> vec(N);
    vector<int> vec2;
    map<int, int> coMap;

    for (int i = 0; i < N; i++) {
        cin >> vec[i];
        vec2.push_back(vec[i]);
    }

    sort(vec2.begin(), vec2.end());
    vec2.erase(unique(vec2.begin(), vec2.end()), vec2.end());

    for (int i = 0; i < vec2.size(); i++) {
        coMap[vec2[i]] = i;
    }

    for (int i = 0; i < N; i++) {
        cout << coMap[vec[i]] << " ";
    }


    return 0;
}
