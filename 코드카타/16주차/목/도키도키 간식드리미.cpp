// https://www.acmicpc.net/problem/12789
#include <iostream>
#include <queue>
#include <stack>

using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(NULL);

    int N;
    cin >> N;
    queue<int> q1;
    
    for (int i = 0; i < N; i++)
    {
        int tmp;
        cin >> tmp;
        q1.push(tmp);
    }

    stack <int> q2;
    
    int target = 1;
    stack<int> q3;
    
    while (!q1.empty())
    {
        int current = q1.front();
        if (current == target)
        {
            target++;
            q1.pop();
        } else
        {
            if (!q2.empty() && q2.top() == target)
            {
                target++;
                q2.pop();
            } else
            {
                q1.pop();
                q2.push(current);
            }
        }
    }
    bool flag = false;
    while (!q2.empty())
    {
        int current = q2.top();
        if (current == target)
        {
            target++;
            q2.pop();
        } else
        {
            flag = true;
            break;
        }
    }
    
    if (flag)
    {
        cout << "Sad" <<"\n";
    } else
    {
        cout << "Nice" << "\n";
    }
    
    return 0;
}