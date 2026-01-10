#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(NULL);
    int testCase = 1;
    while (true) {
        string A;
        string B;
        cin >> A;
        cin >> B;
        if (A == "END" && B == "END") {
            break;
        }
        sort(A.begin(), A.end());
        sort(B.begin(), B.end());
        if (A == B) {
            cout << "Case " << testCase++ << ": same" << "\n";
        } else {
            cout << "Case " << testCase++ << ": different" << "\n";
        }
    }

    return 0;
}