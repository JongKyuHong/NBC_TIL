// Copyright JongKyu

#include <iostream>
#include <queue>

using namespace std; //NOLINT

int main() {
    int n;
    cin >> n;

    priority_queue<int> pq;

    for (int i = 0; i < n; i++) {
        int a;
        cin >> a;

        if (a == 0) {
            if (pq.empty()) {
                cout << -1 << "\n";
            }
            else {
                cout << pq.top() << "\n";
                pq.pop();
            }
        }
        else {
            for (int j = 0; j < a; j++) {
                int tmp;
                cin >> tmp;
                pq.push(tmp);
            }
        }
    }

    return 0;
}
