#include <iostream>
#include <vector>

using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(NULL);
    
    int n, m;

    while (cin >> n >> m && (n != 0 || m != 0)) {
        vector<int> prices(n);
        for (int i = 0; i < n; i++) {
            cin >> prices[i];
        }

        int max_sum = -1;

        for (int i = 0; i < n; i++) {
            for (int j = i + 1; j < n; j++) {
                int current_sum = prices[i] + prices[j];
                if (current_sum <= m) {
                    if (current_sum > max_sum) {
                        max_sum = current_sum;
                    }
                }
            }
        }

        if (max_sum == -1) {
            cout << "NONE" << endl;
        } else {
            cout << max_sum << endl;
        }
    }
    
    return 0;
}