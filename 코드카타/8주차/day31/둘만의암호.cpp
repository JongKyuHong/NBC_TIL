#include <string>
#include <vector>
#include <iostream>

using namespace std;

string solution(string s, string skip, int index) {
    string answer = "";
    vector<char> vec(26, 'a');
    vector<bool> isSkip(26, false);
    for (int i = 0; i < 26; i++){
        vec[i] = 'a' + i;
    }
    
    for (int i = 0; i < 26; i++){
        if (skip.find(vec[i]) != string::npos){
            isSkip[i] = true;
        }
    }
    
    for (int i = 0; i < s.length(); i++){
        char current = s[i];
        int tmp = 0;
        while (tmp < index){
            current++;
            if (current > 'z'){
                current = 'a';
            } 
            if (!isSkip[current-'a']){
                tmp++;                
            }
        }
        answer += current;
    }
    
    return answer;
}