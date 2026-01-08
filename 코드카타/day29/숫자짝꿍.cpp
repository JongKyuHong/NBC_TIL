#include <string>
#include <vector>

using namespace std;

string solution(string X, string Y) {
    string answer = "";
    int cntX[10] = {0};
    int cntY[10] = {0};
    
    for (char xChar : X){
        cntX[xChar-'0']++;
    }
    
    for (char yChar : Y){
        if (cntX[yChar-'0'] > 0){
            cntY[yChar-'0']++;
            cntX[yChar-'0']--;
        }
    }
    
    for (int i = 9; i >= 0; i--){
        answer.append(cntY[i], char('0'+i));
    }
    if (answer.length() == 0){
        answer = "-1";
    } 
    if (answer[0] == '0'){
        answer = "0";
    }
    return answer;
}