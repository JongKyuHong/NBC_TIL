#include <string>
#include <vector>
#include <algorithm>

using namespace std;

string solution(string s) {
    string answer = "";
    bool flag = false;
    for (int i = 0; i < s.length(); i++){
        if (s[i] == ' '){
            flag = false;
            answer += " ";
        } else if (flag){
            answer += tolower(s[i]);
            flag = !flag;
        } else {
            answer += toupper(s[i]);
            flag = !flag;
        }
    }
    
    
    return answer;
}