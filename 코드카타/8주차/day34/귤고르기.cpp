#include <string>
#include <vector>
#include <unordered_map>
#include <algorithm>

using namespace std;

int solution(int k, vector<int> tangerine) {
    int answer = 0;
    unordered_map<int, int> countTan;
    for (int tan : tangerine) {
        countTan[tan]++;
    }
    
    vector<int> counts;
    
    for (auto& countT : countTan){
        counts.push_back(countT.second);
    }
    
    sort(counts.begin(), counts.end(), greater<int>());
    
    for (int count : counts){
        k -= count;
        answer++;
        if (k <= 0){
            break;
        }
    }
    return answer;
}