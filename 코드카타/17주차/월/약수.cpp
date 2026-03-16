// https://www.acmicpc.net/problem/1037

#include <algorithm>
#include <iostream>
#include <vector>

using namespace std;


int main() {
    ios::sync_with_stdio(false);
    cin.tie(NULL);

    int N = 0;
    int countNum;
    cin >> countNum;
    vector<int> vec(countNum);
    for (int i = 0; i < countNum; i++)
    {
        cin >> vec[i];
    }
    
    sort(vec.begin(), vec.end());
    if (vec.size() == 1)
    {
        cout << vec[0]*vec[0];
    } else
    {
        cout << vec[0] * vec[countNum - 1];    
    }
    
    return 0;
}