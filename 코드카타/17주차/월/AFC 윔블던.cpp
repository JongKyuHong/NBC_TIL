// https://www.acmicpc.net/problem/4299

#include <iostream>

using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(NULL);

    int sumV, diff;
    cin >> sumV >> diff;
    if (diff > sumV)
    {
        cout << -1;
        return 0;
    }
    
    int A = (sumV + diff) / 2;
    int B = (sumV - diff) / 2;
    
    if (A + B != sumV || A - B != diff) {
        cout << -1;
        return 0;
    }
    
    cout << A << " " << B;
    
    return 0;
}