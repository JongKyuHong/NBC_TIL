#include <string>
#include <vector>
#include <iostream>

using namespace std;

string solution(string s, int n) {
    string answer = "";
    
    for (int i = 0; i < s.length(); i++){
        if (s[i] == ' '){
            answer += " ";
            continue;
        }
        if (s[i] >= 'A' && s[i] <= 'Z'){
            answer += char((s[i]-'A'+n)%26 + 'A');
        } else {
            answer += char((s[i]-'a'+n)%26 + 'a');
        }
        
    }
    
    return answer;
}