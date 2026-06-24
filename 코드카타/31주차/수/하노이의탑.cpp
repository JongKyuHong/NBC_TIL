// https://school.programmers.co.kr/learn/courses/30/lessons/12946

#include <string>
#include <vector>

using namespace std;

void move(int start, int end, int mid, int n, vector<vector<int>>& answer){
    if (n == 1){
        answer.push_back({start, end});
        return;
    }
    // 가장 큰 원판을 제외하고 모두 start에서 mid로 옮김
    move(start, mid, end, n-1, answer);
    // 가장 큰 원판을 start에서 end로 옮김
    answer.push_back({start, end});
    // 나머지를 다시 end로 옮김
    move(mid, end, start, n-1, answer);
}

vector<vector<int>> solution(int n) {
    vector<vector<int>> answer;
    
    move(1, 3, 2, n, answer);
    
    return answer;
}