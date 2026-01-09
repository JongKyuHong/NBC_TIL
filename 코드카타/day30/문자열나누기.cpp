#include <string>
#include <vector>

using namespace std;

int solution(string s) {
    int answer = 0;
    
    char prev = '\0';
    int count = 0;
    int otherCount = 0;
    int index = 0;
    for (int i = 0; i < s.length(); i++){
        if (prev == '\0'){
            prev = s[i];
            count++;
        } else {
            if (prev != s[i]){
                otherCount++;
                if (count == otherCount){
                    count = 0;
                    otherCount = 0;
                    prev = '\0';
                    answer++;
                    index = i;
                }
            } else {
                count++;
            }
        }
    }
    
    if (count != 0){
        answer++;
    }
    
    return answer;
}