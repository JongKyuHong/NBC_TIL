#include <iostream>
#include <vector>
#include <algorithm>
#include <string>

using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(NULL);

    int n;
    cin >> n;

    vector<int> vec(n);
    for (int i = 0; i < n; i++) {
        cin >> vec[i];
    }

    int x;
    cin >> x;

    sort(vec.begin(), vec.end());

    int start = 0;
    int end = n - 1;  // 오른쪽 끝에서 시작
    int cnt = 0;

    while (start < end) {  // start와 end가 만날 때까지
        int sum = vec[start] + vec[end];

        if (sum == x) {
            cnt++;
            start++;  // 양쪽 모두 이동
            end--;
        }
        else if (sum < x) {
            start++;  // 합이 작으면 start를 오른쪽으로
        }
        else {  // sum > x
            end--;  // 합이 크면 end를 왼쪽으로
        }
    }

    cout << cnt;

    return 0;
}
