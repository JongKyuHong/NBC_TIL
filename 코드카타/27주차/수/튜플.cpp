// https://school.programmers.co.kr/learn/courses/30/lessons/64065?language=cpp#

#include <string>
#include <vector>
#include <algorithm>
#include <unordered_map>
#include <iostream>

using namespace std;

vector<int> solution(string s) {
    vector<int> answer;
    vector<vector<int>> strVec;
    
    bool openVec = false;
    vector<int> tmp;
    string tmpStr = "";
    for (int i = 1; i < s.length()-1; i++){
        if (s[i] == '{'){
            continue;
        } else if (s[i] == ','){
            if (tmpStr.length() > 0){
                tmp.push_back(stoi(tmpStr));
                tmpStr = "";    
            }
        } else if (s[i] == '}'){
            tmp.push_back(stoi(tmpStr));
            tmpStr = "";
            strVec.push_back(tmp);
            tmp = {};
        } else {
            tmpStr += s[i];
        }
    }
    
    sort(strVec.begin(), strVec.end(), [](const auto& a, const auto& b){
        if (a.size() != b.size()) return a.size() < b.size(); 
    });
    
    // vec에 해당 원소가 몇개있는지 보고, answer에 없으면 추가??
    
    for (auto& vec : strVec){
        for (int v : vec){
            int cnt = count(vec.begin(), vec.end(), v);
            int answerCnt = count(answer.begin(), answer.end(), v);
            if (cnt > answerCnt){
                answer.push_back(v);
            }
        }
    }

    return answer;
}