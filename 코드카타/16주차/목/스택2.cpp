// https://www.acmicpc.net/problem/28278

#include <iostream>
#include <stack>

using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(NULL);

    int N;
    cin >> N;
    stack<int> st;
    for (int i = 0; i < N; i++)
    {
        int cmd;
        cin >> cmd;
        if (cmd == 1)
        {
            int num;
            cin >> num;
            st.push(num);
        } else if (cmd == 2)
        {
            if (st.empty())
            {
                cout << -1 << "\n";
            } else
            {
                cout << st.top() << "\n";
                st.pop();
            }
        } else if (cmd == 3)
        {
            cout << st.size() << "\n";
        } else if (cmd == 4)
        {
            if (st.empty())
            {
                cout << 1 << "\n";
            } else
            {
                cout << 0 << "\n";
            }
        } else
        {
            if (st.empty())
            {
                cout << -1 << "\n";
            } else
            {
                cout << st.top() << "\n";
            }
        }
    }
    
    return 0;
}