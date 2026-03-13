// https://www.acmicpc.net/problem/2346

#include <iostream>
#include <vector>

using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(NULL);

    int N;
    cin >> N;
    
    vector<pair<int, int>> vec(N);
    
    for (int i = 0; i < N; i++)
    {
        int tmp;
        cin >> tmp;
        vec[i].first = tmp;
        vec[i].second = i+1;
    }
    int index = 0;
    
    while (true)
    {
        cout << vec[index].second << " ";
        int current = vec[index].first;
        vec.erase(vec.begin() + index);
        if (vec.empty()) break;
        int size = (int)vec.size();
        if (current > 0)
        {
            index = (index+current-1) % size;    
        } else
        {
            index = ((index+current) % size + size) % size;
        }
    }
   
    return 0;
}