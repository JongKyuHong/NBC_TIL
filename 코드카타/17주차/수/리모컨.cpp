// https://www.acmicpc.net/problem/1107

#include <iostream>
#include <vector>
#include <algorithm>
#include <string>

using namespace std;

int minValue;
vector<int> buttons(10, 0);

void recursion(const string& tmp, const string& N)
{
    if (!tmp.empty())
    {
        minValue = min(minValue, abs(stoi(N)-stoi(tmp)) + (int)tmp.length());
    }
    
    if ((int)tmp.length() >= (int)N.length() + 1) return;
    
    for (int i = 0; i <= 9; i++)
    {
        if (buttons[i] == 0)
        {
            recursion(tmp + to_string(i), N);
        }
    }
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(NULL);
   
    string N;
    cin >> N;
    int M;
    cin >> M;

    for (int i = 0; i < M; i++)
    {
        int inp;
        cin >> inp;
        buttons[inp] = 1;
    }

    minValue = abs(stoi(N)-100);
    recursion("", N);
    
    cout << minValue;

    return 0;
}