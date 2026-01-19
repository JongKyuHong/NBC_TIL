#include <string>
#include <vector>
#include <unordered_map>
#include <iostream>

using namespace std;

int solution(vector<string> want, vector<int> number, vector<string> discount) {
    int answer = 0;
    unordered_map<string, int> counts;
    unordered_map<string, int> goal;
    
    
    for (int i = 0; i < want.size(); i++){
        goal[want[i]] = number[i];
    }
    
    for (int i = 0; i < 10; i++){
        counts[discount[i]]++;
    }
    
    if (goal == counts) {
        answer++;
    }
    for (int i = 10; i < discount.size(); i++){
        counts[discount[i-10]]--;
        counts[discount[i]]++;
        
        if (counts[discount[i-10]] == 0){
            counts.erase(discount[i-10]);
        }
        
        if (goal == counts){
            answer++;
        }
    }
    
    return answer;
}