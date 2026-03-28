#include <iostream>
#include <algorithm>

using namespace std;

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    long long A, B;
    cin >> A >> B;

    if (A > B) swap(A, B);

    long long n = B - A + 1;
    long long result = n * (A + B) / 2;

    cout << result << endl;

    return 0;
}