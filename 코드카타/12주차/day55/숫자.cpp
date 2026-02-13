#include <iostream>
#include <algorithm>

using namespace std;

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    long long A, B;
    cin >> A >> B;

    if (A > B) swap(A, B);

    if (A == B) {
        cout << 0 << "\n";
    } else {
        cout << B - A - 1 << "\n";
        for (long long i = A + 1; i < B; i++) {
            cout << i << " ";
        }
    }

    return 0;
}