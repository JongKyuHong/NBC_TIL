// https://school.programmers.co.kr/learn/courses/30/lessons/389479?language=cpp

#include <string>
#include <vector>
#include <queue>
#include <iostream>

using namespace std;

int solution(vector<int> players, int m, int k) {
    int answer = 0;
    
    // 서버의 수명을 어디서 관리할것인가 서버의 총 수를 다른곳에서 계산하고
    queue<int> q;
    
    for (int i = 0; i < k; i++){
        q.push(0);
    }
    
    int currentServerCnt = 0;
    for (const int& player : players){
        currentServerCnt -= q.front();
        q.pop();
        // 현재 필요한 서버 갯수
        int tmp = player / m;
        // 현재 서버 수 보다 필요한게 더 많다면
        if (tmp > currentServerCnt){
            // 증설
            q.push(tmp-currentServerCnt);
            // 증설한 수 만큼 추가
            answer += tmp-currentServerCnt;
            // 현재 수 갱신
            currentServerCnt = tmp;
        } else {
            // 서버수가 충분하다면 0넣음
            q.push(0);
        }
    }
    
    return answer;
}