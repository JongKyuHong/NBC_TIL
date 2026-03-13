// https://www.acmicpc.net/problem/28279

#include <iostream>
#include <deque>

using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(NULL);

    deque<int> dq;
    
    int N;
    cin >> N;
    
    for (int i = 0; i < N; i++)
    {
        int cmd;
        cin >> cmd;
        if (cmd == 1)
        {
            int num;
            cin >> num;
            dq.push_front(num);
        } else if (cmd == 2)
        {
            int num;
            cin >> num;
            dq.push_back(num);
        } else if (cmd == 3)
        {
            if (dq.empty()) cout << -1 << "\n";
            else
            {
                cout << dq.front() << "\n";
                dq.pop_front();
            }
        } else if (cmd == 4)
        {
            if (dq.empty()) cout << -1 << "\n";
            else
            {
                cout << dq.back() << "\n";
                dq.pop_back();
            }
        } else if (cmd == 5)
        {
            cout << dq.size() << "\n";
        } else if (cmd == 6)
        {
            if (dq.empty()) cout << 1 << "\n";
            else cout << 0 << "\n";
        } else if (cmd == 7)
        {
            if (dq.empty()) cout << -1 << "\n";
            else cout << dq.front() << "\n";
        } else if (cmd == 8)
        {
            if (dq.empty()) cout << -1 << "\n";
            else cout << dq.back() << "\n";
        }
    }
    
    return 0;
}