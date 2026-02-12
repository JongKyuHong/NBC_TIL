# 코드카타

## 이모티콘 할인행사

- [이모티콘 할인행사](https://school.programmers.co.kr/learn/courses/30/lessons/150368?language=cpp)

```c++
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
```

- 별로 어려운문제는 아니였는데 처음에 접근방법을 잘못잡아서 포기하고 다시 돌아오는게 오래걸렸다.
- 전역변수로 배열을 만들고 할인율만 저장하여 계산하였다.
