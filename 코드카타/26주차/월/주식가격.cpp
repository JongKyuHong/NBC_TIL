// https://school.programmers.co.kr/learn/courses/30/lessons/42584?language=cpp

#include <string>
#include <vector>
#include <iostream>

using namespace std;

vector<int> solution(vector<int> prices) {
    int N = prices.size();
    vector<int> answer(N, 0);
    
    vector<pair<int, int>> stack;
    
    for (int i = 0; i < N; i++){
        if (stack.empty() || stack[stack.size()-1].second < prices[i]){
            stack.push_back({i, prices[i]});    
        } else {
            while (!stack.empty() && stack[stack.size()-1].second > prices[i]){
                answer[stack[stack.size()-1].first] = i - stack[stack.size()-1].first;
                stack.pop_back();
            }
            stack.push_back({i, prices[i]});    
        }
    }
    
    while (!stack.empty()){
        answer[stack[stack.size()-1].first] = N - 1 - stack[stack.size()-1].first;
        stack.pop_back();
    }
    return answer;
}