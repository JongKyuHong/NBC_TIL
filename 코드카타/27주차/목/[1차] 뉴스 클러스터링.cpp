// https://school.programmers.co.kr/learn/courses/30/lessons/17677?language=cpp#

#include <string>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <algorithm>
#include <cmath>
#include <iostream>

using namespace std;

int solution(string str1, string str2) {
    int answer = 0;
    
    // 부분집합이 들어갈 벡터
    unordered_map<string, int> um1;
    unordered_map<string, int> um2;
    
    for (int i = 0; i < str1.length()-1; i++){
        char s1 = tolower(str1[i]);
        char s2 = tolower(str1[i+1]);
        if (isalpha(s1) && isalpha(s2)){
            string tmp = "";
            tmp += s1;
            tmp += s2;
            um1[tmp]+=1;
        }
    }
    
    for (int i = 0; i < str2.length()-1; i++){
        char s1 = tolower(str2[i]);
        char s2 = tolower(str2[i+1]);
        if (isalpha(s1) && isalpha(s2)){
            string tmp = "";
            tmp += s1;
            tmp += s2;
            um2[tmp]+=1;
        }
    }
    
    unordered_set<string> unionSet;
    unordered_set<string> interSet;
    vector<string> vec1;
    vector<string> vec2;
    
    for (auto [key, value] : um1){
        for (int i = 0; i < value; i++){
            string tmp = key + to_string(i);
            vec1.push_back(tmp);
            unionSet.insert(tmp);
        }
    }

    for (auto [key, value] : um2){
        for (int i = 0; i < value; i++){
            string tmp = key + to_string(i);
            vec2.push_back(tmp);    
            unionSet.insert(tmp);
        }
    }

    for (auto v : vec1){
        if (count(vec2.begin(), vec2.end(), v)){
            interSet.insert(v);
        }
    }

    return unionSet.size() == 0 ? 65536 : floor(((double)interSet.size() / (double)unionSet.size()) *65536);
}