// https://school.programmers.co.kr/learn/courses/30/lessons/17686?language=cpp

#include <string>
#include <vector>
#include <algorithm>
#include <iostream>

using namespace std;

vector<string> convert(const string& file){
    string head = "";
    string number = "";
    string tail = "";
    bool isString = true;
    bool isNumber = false;
    bool isTail = false;
    for (const char& f : file){
        if (isString){
            if (isdigit(f)){
                isString = false;
                isNumber = true;
                number += f;
            } else {
                head += tolower(f);    
            }
        } else if (isNumber){
            if (isdigit(f) && number.length() < 5){
                number += f;
            } else {
                isNumber = false;
                isTail = true;
                tail += f;
            }
        } else if (isTail){
            tail += f;
        }   
    }
    
    return {head, number, tail};
}

vector<string> solution(vector<string> files) {
    vector<string> answer;
    
    for (const string& file : files){
        answer.push_back(file);
    }
    
    stable_sort(answer.begin(), answer.end(), [](auto& a, auto& b){
        vector<string> av = convert(a);
        vector<string> bv = convert(b);
        
        int ia = stoi(av[1]);
        int ib = stoi(bv[1]);
        
        if (av[0] != bv[0]) return av[0] < bv[0];
        else if (ia != ib) return ia < ib;
        return false;
    });

    
    return answer;
}