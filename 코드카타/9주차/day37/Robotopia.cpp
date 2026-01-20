#include <iostream>

using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(NULL);

    int N;
    cin >> N;

    while (N--) {
        int l1, a1, l2, a2, l3, a3;
        cin >> l1 >> a1 >> l2 >> a2 >> l3 >> a3;

        int finalX = 0, finalY = 0;
        int count = 0;
        for (int x = 1; l1 * x < l3; x++) {
            int remainingL = l3 - (l1 * x);
            if (remainingL % l2 == 0) {
                int y = remainingL / l2;
                if (y >= 1) {
                    if (a1 * x + a2 * y == a3) {
                        count++;
                        finalX = x;
                        finalY = y;
                    }
                }
            }
            if (count > 1) break;
        }

        if (count == 1) {
            cout << finalX << " " << finalY << "\n";
        } else {
            cout << "?" << "\n";
        }
    }

    return 0;
}