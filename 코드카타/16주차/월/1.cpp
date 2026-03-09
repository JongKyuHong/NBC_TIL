// https://www.acmicpc.net/problem/4375

#include <iostream>
#include <string>

using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(NULL);
    
    int n;
    while (cin >> n)
    {
        long long current = 1;
        int ans = 1;

        while (true)
        {
            if (current % n == 0)
            {
                cout << ans << "\n";
                break;
            }
            current = (current * 10 + 1) % n;
            ans++;
        }
    }
}