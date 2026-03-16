// https://www.acmicpc.net/problem/25501

#include <iostream>

using namespace std;

int recursion(const string& s, int l, int r, int& count)
{
    count++;
    if (l >= r) return 1;
    else if (s[l] != s[r]) return 0;
    else return recursion(s, l+1, r-1, count);
}

pair<int, int> isPalindrome(const string& s)
{
    int count = 0;
    int result = recursion(s, 0, s.length()-1, count);
    return {result, count};
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(NULL);

    int T;
    cin >> T;
    
    for (int i = 0; i < T; i++)
    {
        string S;
        cin >> S;
        pair<int, int> result = isPalindrome(S);
        cout << result.first << " " << result.second << "\n";
    }
    
    return 0;
}