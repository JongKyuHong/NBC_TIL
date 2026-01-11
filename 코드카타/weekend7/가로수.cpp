#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

long long gcd(long long a, long long b) {
    while (b != 0) {
        int r = a % b;
        a = b;
        b = r;
    }
    return a;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(NULL);
    int answer = 0;

    int N;
    cin >> N;

    vector<long long> vec(N);
    long long prev = -1;
    long long minV = 1000000001;
    long long prevGap = -1;
    for (int i = 0; i < N; i++) {
        long long inp;
        cin >> inp;
        if (prev == -1) {
            prev = inp;
        } else {
            long long gap = inp - prev;
            if (prevGap == -1) {
                prevGap = gap;
            } else {
                long long tmp = gcd(gap, prevGap);
                prevGap = tmp;
            }
            prev = inp;
            vec[i] = gap;
        }
    }

    for (int i = 1; i < vec.size(); i++) {
        answer += (vec[i] / prevGap) - 1;
    }
    cout << answer;

    return 0;
}
