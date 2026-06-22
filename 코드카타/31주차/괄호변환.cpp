// https://school.programmers.co.kr/learn/courses/30/lessons/60058?language=cpp

#include <string>
#include <vector>

using namespace std;

bool isRight(string& p){
    vector<char> vec;
    for (const char& c : p){
        if (c == '('){
            vec.push_back(c);
        } else {
            if (!vec.empty() && vec.back() == '('){
                vec.pop_back();
            }
        }
    }
    return vec.empty() ? true : false;
}

bool isEqualBalance(string& p){
    int open = 0;
    int close = 0;
    for (const char& c : p){
        if (c == '('){
            open++;
        } else {
            close++;
        }
    }
    return open == close ? true : false;
}

string makeString(string p){
    if (p.empty()) return "";
    
    string u = "";
    string v = "";
    string result = "";
    int index = 0;
    while (index < p.size()){
        u += p[index++];
        if (isEqualBalance(u)){
            v = p.substr(index);
            break;
        }
    }
    
    if (isRight(u)){
        return u + makeString(v);
    } else {
        string tmp = "(" + makeString(v) + ")";
        u.erase(u.begin());
        u.erase(u.end()-1);
        for (char& c : u){
            c = (c == '(') ? ')' : '(';
        }
        tmp += u;
        return tmp;
    }
}

string solution(string p) {
    string answer = "";
    
    // 모든 괄호를 뽑아서 올바른 순서대로 배치된 괄호 문자열을 알려주는 프로그램
    // 균형잡힌, 올바른
    
    return makeString(p);
}