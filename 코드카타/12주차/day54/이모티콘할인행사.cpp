#include <string>
#include <vector>
#include <algorithm>
#include <iostream>

using namespace std;

int maxPlus = 0;
int maxMoney = 0;
vector<int> disCounts;

vector<int> calculate(){
    
}

void dfs(int depth, vector<vector<int>>& users, vector<int>& emoticons){
    if (depth == emoticons.size()){
        int totalPlus = 0;
        int totalMoney = 0;
        for (auto user : users){
            int sumV = 0;
            for (int i = 0; i < emoticons.size(); i++){
                if (disCounts[i] >= user[0]){
                    sumV += (emoticons[i]*(100-disCounts[i])) / 100;
                }
            }
            if (sumV >= user[1]){
                sumV = 0;
                totalPlus++;
            } else {
                totalMoney += sumV;
            }
        }
        if (maxPlus < totalPlus){
            maxPlus = totalPlus;
            maxMoney = totalMoney;
        } else if (maxPlus == totalPlus){
            maxMoney = max(maxMoney, totalMoney);
        }
        return;
    }
    
    for (int i = 0; i < 4; i++){
        disCounts.push_back((i+1)*10);
        dfs(depth+1, users, emoticons);
        disCounts.pop_back();
    }
}

vector<int> solution(vector<vector<int>> users, vector<int> emoticons) {
    vector<int> tmp;

    dfs(0, users, emoticons);

    return {maxPlus, maxMoney};
}