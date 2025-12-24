#include <string>
#include <vector>
#include <iostream>

using namespace std;

int solution(string t, string p) {
    int answer = 0;
    int len = p.length();
    int start = 0;
    int end = 0;
    string tmp = "";
    long long target = stoll(p);
    while (end <= t.length()){
        if (tmp.length() < len){
            tmp += t[end++];
        } else {
            if (stoll(tmp) <= target){
                answer++;
            }
            tmp.erase(tmp.begin(), tmp.begin()+1);
            start++;
        }
    }
    
    return answer;
}