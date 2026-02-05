#include <string>
#include <vector>
#include <unordered_map>
#include <iostream>

using namespace std;

int answer;

void dfs(vector<int>& picks, int index, int remain, int current, vector<string>& minerals, vector<unordered_map<string, int>>& fatigue, int total, int total_picks){
    if (index >= minerals.size() || (total_picks == 0 && remain == 0)){
        answer = min(answer, total);
        return;
    }
    if (remain > 0){
        // 현재 곡괭이로 광물을 캤을때의 피로도
        dfs(picks, index+1, remain-1, current, minerals, fatigue, total + fatigue[current][minerals[index]], total_picks);
    } else {
        // 곡괭이를 다썼다면 다른거로 바꾸는 시간
        for (int i = 0; i < 3; i++){
            if (picks[i] > 0){
                picks[i]--;
                dfs(picks, index, 5, i, minerals, fatigue, total,total_picks - 1);
                picks[i]++;
            }
        }
    }
}

int solution(vector<int> picks, vector<string> minerals) {
    answer = 10000;
    vector<unordered_map<string, int>> fatigue(3);
    
    fatigue[0]["diamond"] = 1;
    fatigue[0]["iron"] = 1;
    fatigue[0]["stone"] = 1;
    fatigue[1]["diamond"] = 5;
    fatigue[1]["iron"] = 1;
    fatigue[1]["stone"] = 1;
    fatigue[2]["diamond"] = 25;
    fatigue[2]["iron"] = 5;
    fatigue[2]["stone"] = 1;
    
    int total_picks = 0;
    for (int i = 0; i < 3; i++){
        total_picks += picks[i];
    }
    
    for (int i = 0; i < 3; i++){
        if (picks[i] > 0){
            picks[i]--;
            dfs(picks, 0, 5, i, minerals, fatigue, 0, total_picks - 1);
            picks[i]++;
        }
    }

    return answer;
}