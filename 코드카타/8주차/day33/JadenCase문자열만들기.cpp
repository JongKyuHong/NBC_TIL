#include <string>
#include <vector>
#include <sstream>

using namespace std;

string solution(string s) {
    string answer = "";
    
    bool isFirst = true;
    for (int i = 0; i < s.length(); i++){
        if (isFirst) {
            if (isalpha(s[i])){
                answer += toupper(s[i]);
            } else {
                answer += s[i];
            }
            isFirst = false;
        } else {
            if (isalpha(s[i])){
                answer += tolower(s[i]);    
            } else {
                answer += s[i];    
            }
        }   
        if (isspace(s[i])){
            isFirst = true;
        }
    }
    
    return answer;
}