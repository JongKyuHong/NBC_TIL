// https://school.programmers.co.kr/learn/courses/30/lessons/1835

#include <string>
#include <vector>
#include <iostream>
#include <cmath>

using namespace std;

string standard = "ACFJMNRT";
int total = 0;

void dfs(int depth, string tmp, vector<bool>& visited, vector<string>& data){
    if (depth == 8){
        bool flag = true;
        for (int i = 0; i < data.size(); ++i){
            char a = data[i][0];
            char b = data[i][2];
            char op = data[i][3];
            int value = data[i][4] - '0';
            
            int gap = abs((int)tmp.find(a) - (int)tmp.find(b)) - 1;
            
            if (op == '='){
                if (gap != value){
                    flag = false;
                    break;
                }    
            } else if (op == '<'){
                if (gap >= value){
                    flag = false;
                    break;
                }
            } else if (op == '>'){
                if (gap <= value){
                    flag = false;
                    break;
                }
            }
        }
        if (flag) total++;
        return;
    }
    
    for (int i = 0; i < 8; ++i){
        if (!visited[i]){
            visited[i] = true;
            dfs(depth+1, tmp + standard[i], visited, data);
            visited[i] = false;
        }
    }
}

// 전역 변수를 정의할 경우 함수 내에 초기화 코드를 꼭 작성해주세요.
int solution(int n, vector<string> data) {
    // 모든 경우를 다 탐색해야 하는 완전 탐색 -> dfs로 다 돌려볼 수 밖에 없음
    total = 0;
    // 배치했는지 {A, C, F, J, M, N, R, T}
    vector<bool> visited(8, false);
    for (int i = 0; i < 8; ++i){
        // 첫번째 자리, 들어올 캐릭터, 조건 확인을 위한 data 넘김
        visited[i] = true;
        dfs(1, string(1, standard[i]), visited, data);
        visited[i] = false;
    }
    return total;
}