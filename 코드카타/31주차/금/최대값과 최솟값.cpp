// https://school.programmers.co.kr/learn/courses/30/lessons/12939

#include <string>
#include <vector>
#include <sstream>
#include <algorithm>
#include <climits>

using namespace std;

string solution(string s) {
    stringstream ss(s);
    string word;

    int minV = INT_MAX;
    int maxV = INT_MIN;
    
    while (ss >> word){
        minV = min(minV, stoi(word));
        maxV = max(maxV, stoi(word));
    }
    
    return to_string(minV) + " " + to_string(maxV);
}