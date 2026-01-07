#include <iostream>
#include <numeric>  

using namespace std;

long long gcd_ll(long long a, long long b) {
    while (b != 0) {
        long long r = a % b;
        a = b;
        b = r;
    }
    return a;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    long long A, B;
    cin >> A >> B;

    long long g = gcd_ll(A, B);
    long long lcm = (A / g) * B;

    cout << lcm << "\n";
    return 0;
}