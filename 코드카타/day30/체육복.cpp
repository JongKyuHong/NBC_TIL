#include <string>
#include <vector>
#include <algorithm>

using namespace std;

int solution(int n, vector<int> lost, vector<int> reserve) {
    int answer = 0;
    sort(lost.begin(), lost.end());
    
    for (int& l : lost){
        auto it = find(reserve.begin(), reserve.end(), l);
        if (it != reserve.end()){
            reserve.erase(it);
            l = 0;
        }
    }
    
    for (int l : lost){
        if (l == 0){
            continue;
        }
        auto it = find(reserve.begin(), reserve.end(), l-1);
        if (it != reserve.end()){
            reserve.erase(it);
            continue;
        }
        auto it2 = find(reserve.begin(), reserve.end(), l+1);
        if (it2 != reserve.end()){
            reserve.erase(it2);
            continue;
        }
        answer++;
    }
    
    return n-answer;
}