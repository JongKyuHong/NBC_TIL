#include <iostream>
#include <map>
#include <algorithm>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(NULL);

    int N;
    cin >> N;

    // 수안에 2x5가 몇번 나오는가
    map<int, int> mm;
    mm.insert({ 2, 0 });
    mm.insert({ 5, 0 });
    int cnt = 0;
    for (int i = 1; i <= N; i++) {
        int value = i;
        while (value % 2 == 0) {
            value /= 2;
            mm[2] += 1;
        }
        while (value % 5 == 0) {
            value /= 5;
            mm[5] += 1;
        }
    }

    int res = min(mm[2], mm[5]);
    

    cout << res;

    return 0;
}
