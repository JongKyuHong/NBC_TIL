#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(NULL);

    int N;
    cin >> N;

    vector<int> vec(N);
    vector<int> sorted_vec;  // 정렬된 고유값 저장

    for (int i = 0; i < N; i++) {
        cin >> vec[i];
        sorted_vec.push_back(vec[i]);
    }

    // 1. 정렬
    sort(sorted_vec.begin(), sorted_vec.end());

    // 2. 중복 제거 (erase-unique idiom)
    sorted_vec.erase(unique(sorted_vec.begin(), sorted_vec.end()), sorted_vec.end());

    // 3. 각 원소의 압축 좌표 출력
    for (int i = 0; i < N; i++) {
        // lower_bound로 위치 찾기 - O(log N)
        int index = lower_bound(sorted_vec.begin(), sorted_vec.end(), vec[i]) - sorted_vec.begin();
        cout << index << " ";
    }

    return 0;
}
