#include <string>
#include <vector>

using namespace std;

int solution(vector<string> babbling) {
    int answer = 0;
    string baby[] = {"aya", "ye", "woo", "ma"};
    
    for (int i = 0; i < babbling.size(); i++){
        int index = 0;
        int prev = -1;
        bool flag = true;
        while (index < babbling[i].length()){
            bool check = false;
            for (int j = 0; j < 4; j++){
                if (prev == j){
                    continue;
                }
                if (babbling[i][index] == baby[j][0]){
                    int tmpIndex = 0;
                    bool flag2 = true;
                    while (baby[j].size() > tmpIndex){
                        if (baby[j][tmpIndex] != babbling[i][index+tmpIndex]){
                            flag2 = false;
                            break;
                        }
                        tmpIndex++;
                    }
                    if (flag2){
                        index += tmpIndex;
                        prev = j;
                        check = true;
                    }
                }
            }
            if (!check){
                flag = false;
                break;
            }
        }
        if (flag){
            answer++;
        }
    }
    return answer;
}