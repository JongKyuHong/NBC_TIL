#include <iostream>

using namespace std;

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    long long n;
    cin >> n;

    long long result = (3 * n * n + 5 * n + 2) / 2;
    cout << result % 45678 << endl;

    return 0;
}