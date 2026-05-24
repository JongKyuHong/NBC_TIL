// https://school.programmers.co.kr/learn/courses/30/lessons/12973?language=cpp

#include <iostream>
#include <string>
#include <vector>

using namespace std;

int solution(string s)
{
    vector<char> vec;
    
    for (int i = 0; i < s.length(); i++){
        vec.push_back(s[i]);
        if (vec.size() >= 2){
            auto it = vec.end();
            if (*(it-1) == *(it-2)){
                vec.pop_back();
                vec.pop_back();
            }
        }
    }
    
    return vec.empty() ? 1 : 0;
}