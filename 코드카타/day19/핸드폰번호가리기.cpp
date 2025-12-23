#include <string>
#include <vector>

using namespace std;

string solution(string phone_number) {
    string answer = "";
    int cnt = 0;
    for (int i = phone_number.length() - 1; i >= 0; i--){
        if (cnt >= 4){
            answer = "*" + answer;
        } else {
            answer = phone_number[i] + answer;
            cnt++;
        }
    }
    
    return answer;
}