// https://school.programmers.co.kr/learn/courses/30/lessons/12909

#include <string>
#include <iostream>
#include <vector>

using namespace std;

bool solution(string s)
{
    vector<char> vec;
    
    for (char c : s){
        if (vec.empty()){
            vec.push_back(c);
        } else {
            if (vec.back() == '(' && c == ')'){
                vec.pop_back();
            } else {
                vec.push_back(c);
            }
        }
    }
    
    return vec.empty() ? true : false;
}