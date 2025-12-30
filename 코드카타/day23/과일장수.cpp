#include <string>
#include <vector>
#include <algorithm>
#include <iostream>

using namespace std;

int solution(int k, int m, vector<int> score) {
    int answer = 0;
    
    sort(score.begin(), score.end(), [](const auto& a, const auto& b){
        return a > b;
    });
    
    int index = 0;
    int minV = 10;
    int cnt = 0;
    while (index < score.size()){
        if (score[index] < minV){
            minV = score[index];
        }
        cnt++;
        index++;
        if (cnt == m){          
            answer += minV*cnt;
            cnt = 0;
            minV = 10;
        }
    }
    
    return answer;
}