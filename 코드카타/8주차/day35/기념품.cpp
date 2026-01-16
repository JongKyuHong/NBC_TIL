#include <string>
#include <vector>
#include <iostream>
#include <cmath>

using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(NULL);

    int N;
    cin >> N;

    vector<int> vec(N, 0);

    for (int i = 0; i < N; i++) {
        vec[i] = i + 1;
    }
    
    // N의 세제곱까지 진행된다.
    // 현재 인덱스 + N의 세제곱 했을때 몇번째 인덱스인지
    
    long long index = 0;
    int phase = 1;
    while (true) {
        index += pow(phase++, 3) - 1;
        index %= vec.size();
        if (vec.size() == 1) {
            cout << vec[0];
            break;
        }
        vec.erase(vec.begin()+index);
    }

    return 0;
}
