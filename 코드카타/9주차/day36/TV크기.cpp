#include <iostream>
#include <cmath>

using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(NULL);

    int D, H, W;

    cin >> D >> H >> W;

    // (9x)^2 + (16x)^2 = 52^2;
    // 9 : 16, 18 : 32, 27 : 48
    // 9^2x^2 + 16^2x^2 = 52^2;
    // x^2(9^2 + 16^2) = 52^2;
    // x^2 = 52^2/(9^2 + 16^2);
    

    double x = sqrt(pow(D,2) / (pow(W,2) + pow(H,2)));

    int result1 = int(H * x);
    int result2 = int(W * x);

    cout << result1 << " " << result2 << endl;
    return 0;
}
