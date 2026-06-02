// https://school.programmers.co.kr/learn/courses/30/lessons/49993?language=cpp

#include <string>
#include <vector>
#include <unordered_map>
#include <iostream>

using namespace std;

int solution(string skill, vector<string> skill_trees) {
    int answer = 0;
    unordered_map<char, int> um;
    for (const char& s : skill){
        um[s]++;   
    }
    
    for (const string& skill_tree : skill_trees){
        int index = 0;
        bool isRight = true;
        for (const char& s : skill_tree){
            if (um.find(s) != um.end()){
                if (s == skill[index]){
                    index++;
                } else {
                    isRight = false;
                    break;
                }
            }
        }
        if (isRight){
            // cout << skill_tree << endl;
            answer++;
        }
    }
    
    return answer;
}