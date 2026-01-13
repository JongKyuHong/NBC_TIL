#include <iostream>
#include <algorithm>
#include <vector>

using namespace std;

int countFunctionCall = 0;

long long fibonacci(int N) {
    if (N == 1 || N == 2) {
        countFunctionCall++;
        return 1;
    }
    return fibonacci(N - 1) + fibonacci(N - 2);
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(NULL);
    int N;
    cin >> N;
    long long _ = fibonacci(N);

    cout << countFunctionCall << " " << N - 2;
    return 0;
}
