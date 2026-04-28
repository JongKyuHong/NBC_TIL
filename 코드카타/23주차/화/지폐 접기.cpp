// https://school.programmers.co.kr/learn/courses/30/lessons/340199

#include <string>
#include <vector>

using namespace std;

int solution(vector<int> wallet, vector<int> bill) {
    int answer = 0;
    
    // 길이 긴쪽 반으로, 접은 후 소수점 이하 버림, 접힌 지페를 90도 돌려서 지갑에 넣을 수 있으면 그만
    
    while (true){
        if ((wallet[0] >= bill[0] && wallet[1] >= bill[1]) || (wallet[0] >= bill[1] && wallet[1] >= bill[0])){
            break;
        }   
        
        // 접자
        if (bill[0] > bill[1]){
            bill[0] /= 2;
        } else {
            bill[1] /= 2;
        }
        answer++;
    }
    
    
    return answer;
}