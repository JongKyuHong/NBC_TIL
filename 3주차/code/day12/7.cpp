#include <iostream>
#include <vector>
#include <algorithm>
#include <string>
#include <set>

using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(NULL);

    int N;
    cin >> N;

    vector<int> vec(N);
    set<int> rank;
    
    for (int i = 0; i < N; i++) {
        cin >> vec[i];
        rank.insert(vec[i]);
    }

    for (int i = 0; i < N; i++) {
        int target = vec[i];
        auto it = rank.find(target);
        int index = distance(rank.begin(), it);
        cout << index << " ";
    }
   

    return 0;
}
