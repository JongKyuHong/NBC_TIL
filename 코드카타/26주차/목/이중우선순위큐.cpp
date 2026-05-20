// https://school.programmers.co.kr/learn/courses/30/lessons/42628?language=cpp

#include <string>
#include <vector>
#include <queue>
#include <unordered_map>
#include <sstream>
#include <iostream>

using namespace std;

vector<int> solution(vector<string> operations) {
    vector<int> answer;
    unordered_map<int, int> um;
    
    // 최소 큐
    priority_queue<int, vector<int>, greater<int>> minQ;
    
    // 최대 큐
    priority_queue<int> maxQ;
    
    for (const string& operation : operations){
        char op = operation[0];
        int value = stoi(operation.substr(2));
        if (op == 'I'){
            minQ.push(value);
            maxQ.push(value);
            um[value] += 1;
        } else if (value == 1){
            // 최대값 삭제
            while (!maxQ.empty() && um[maxQ.top()] == 0){
                maxQ.pop();    
            }
            if (!maxQ.empty()){
                um[maxQ.top()] -= 1;
                maxQ.pop();    
            }
        } else if (value == -1){
            while (!minQ.empty() && um[minQ.top()] == 0){
                minQ.pop();    
            }
            if (!minQ.empty()){
                um[minQ.top()] -= 1;
                minQ.pop();    
            }
        }
    }
    while (!maxQ.empty() && um[maxQ.top()] == 0){
        maxQ.pop();    
    }
    while (!minQ.empty() && um[minQ.top()] == 0){
        minQ.pop();    
    }
    if (maxQ.empty() || minQ.empty()){
        answer.push_back(0);
        answer.push_back(0);
    } else {
        answer.push_back(maxQ.top());
        answer.push_back(minQ.top());
    }
    
    
    return answer;
}