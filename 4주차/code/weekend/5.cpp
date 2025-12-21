#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(NULL);

    double N;
    std::cin >> N;

    //값저장
    std::vector<int> vec(N);

    //빈도저장
    std::vector<int> vec2(8001, 0);

    double sumV = 0;
    for (int i = 0; i < N; i++) {
        int inp;
        std::cin >> inp;
        sumV += inp;
        vec[i] = inp;
        vec2[4000 + inp]++;
    }

    sort(vec.begin(), vec.end());

    int av = round(sumV / N);

    std::cout << int(av) << "\n";
    std::cout << vec[N / 2] << "\n";

    int maxV = 0;
    int cnt = -1;
    int index = -1;
    for (int i = 0; i < 8001; i++) {
        if (vec2[i] > maxV) {
            maxV = vec2[i];
            index = i;
            cnt = 1;
        } else if (vec2[i] == maxV) {
            if (cnt < 2) {
                index = i;
                cnt = 2;
            }
        }
    }

    std::cout << index-4000 << "\n";
    std::cout << vec[N - 1] - vec[0] << "\n";

    return 0;
}