#include <string>
#include <vector>
#include <algorithm>

using namespace std;

vector<int> solution(vector<string> keymap, vector<string> targets) {
    vector<int> answer;
    
    for (string& target : targets){
        int sumV = 0;
        bool flag = false;
        for (char t : target){
            int minV = 101;
            for (string& k : keymap){
                auto it = find(k.begin(), k.end(), t);
                if (it == k.end()){
                    continue;
                }
                int value = it - k.begin();
                minV = min(minV, value+1);
            }
            if (minV == 101){
                flag = true;
                break;
            }
            sumV += minV;
        }
        if (flag){
            answer.push_back(-1);
        } else {
            answer.push_back(sumV);
        }
        
    }
    
    return answer;
}