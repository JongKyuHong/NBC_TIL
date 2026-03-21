// https://www.acmicpc.net/problem/4358

#include <iostream>
#include <map>
#include <string>

using namespace std;

int main()
{
    ios_base::sync_with_stdio(false);
    cin.tie(0);

    map<string, int> um;
    int count = 0;
    string inp;
    while (getline(cin, inp))
    {
        if (inp.empty()) break;
        um[inp]++;
        count++;
    }
    
    for (const auto v : um)
    {
        double ratio = (double) v.second*100 / count;
        cout << fixed;
        cout.precision(4);
        cout << v.first << " " << ratio << "\n";
    }
    
    return 0;
}