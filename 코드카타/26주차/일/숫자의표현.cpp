// https://school.programmers.co.kr/learn/courses/30/lessons/12924?language=cpp

#include <string>
#include <vector>

using namespace std;

int solution(int n) {
    int answer = 0;
    for (int i = 1; i <= n; i++){
        int tmp = i;
        int j = i+1;
        while (tmp < n){
            tmp += j++;
        }
        if (tmp == n){
            answer++;
        }
    }
    
    
    return answer;
}