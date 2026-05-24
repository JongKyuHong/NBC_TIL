// https://school.programmers.co.kr/learn/courses/30/lessons/12981?language=cpp

#include <string>
#include <vector>
#include <unordered_map>
#include <iostream>

using namespace std;

vector<int> solution(int n, vector<string> words) {
    vector<int> answer;
    
    // 중복을 확인하기 위한 map을 하나 만듬
    unordered_map<string, int> um;
    
    int index = 0;
    int turn = 1;
    char lastChar = '1';
    for (const string& word : words){
        if (um.find(word) != um.end()){
            answer.push_back(index+1);
            answer.push_back(turn);
            return answer;
        } else {
            if (lastChar == '1' || lastChar == word[0]){
                um[word] = 1;
                lastChar = word[word.length()-1];
                index++;
            } else {
                answer.push_back(index+1);
                answer.push_back(turn);    
                return answer;
            }    
        }
        if (index == n){
            index %= n;
            turn++;
        }
    }
    
    return {0, 0};
}