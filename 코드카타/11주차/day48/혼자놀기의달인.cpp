#include <string>
#include <vector>
#include <algorithm>
#include <set>
#include <iostream>

using namespace std;

int solution(vector<int> cards) {
    int answer = 0;
    
    vector<int> vec;
    vector<int> visited(cards.size(), 0);

    int index = 0;
    for (int i = 0; i < cards.size(); i++){
        if (visited[i]){
            continue;
        }
        int target = i;
        int count = 0;
        while (!visited[target]){
            count++;
            visited[target]++;
            target = cards[target] - 1;
        }
        if (count > 0){
            vec.push_back(count);
        }
    }
    
    if (vec.size() < 2){
        return 0;
    }
    sort (vec.begin(), vec.end(), greater<int>());
    

    return vec[0] * vec[1];
}