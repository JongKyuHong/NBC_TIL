// Copyright JongKyu
#include <iostream>
#include <vector>
#include <algorithm>
#include <map>

using namespace std; //NOLINT

int main() {
    int n;
    cin >> n;

    map<int, int> mm;

    for (int i = 0; i < n; i++) {
        int a;
        cin >> a;

        if (a == 0) {
            if (!mm.empty()) {
                auto tmp = mm.begin();
                if (tmp->second > 0) {
                    tmp->second -= 1;
                    cout << -1*(tmp->first) << "\n";
                    if (tmp->second == 0) {
                        mm.erase(tmp);
                    }
                }
                else {
                    cout << -1 << "\n";
                }
            }
            else {
                cout << -1 << "\n";
            }
        }
        else {
            for (int j = 0; j < a; j++) {
                int tmp;
                cin >> tmp;
                mm[-1*tmp] += 1;
            }
        }
    }
    

    return 0;
}