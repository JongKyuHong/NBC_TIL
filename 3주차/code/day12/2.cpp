#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

int main() {
    cin.tie(NULL);
    ios::sync_with_stdio(false);
    
    int n;
    cin >> n;

    vector<int> vec(n);
    for (int i = 0; i < n; i++) {
        cin >> vec[i];
    }

    sort(vec.begin(), vec.end());

    int cnt = 0;
    int x;
    cin >> x;

    for (int i = 0; i < n - 1; i++) {
        int end = i + 1;

        while (end < n && vec[i] + vec[end] < x) {
            end++;
        }
        if (vec[i] + vec[end] == x) {
            cnt++;
        }
    }

    cout << cnt;

    return 0;
}