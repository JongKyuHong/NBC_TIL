// https://school.programmers.co.kr/learn/courses/30/lessons/42884#

#include <string>
#include <vector>
#include <algorithm>
#include <iostream>

using namespace std;

int solution(vector<vector<int>> routes) {
    int answer = 0;
    
    sort(routes.begin(), routes.end(), [](const auto& a, const auto& b){
       if (a[0] != b[0]) return a[0] > b[0];
        return a[1] > b[1];
    });
    
    
    
    // 모든 차량이 카메라에 걸려야 함
    // 최소개수이므로 최대한 적절한위치에 배치해야한다.
    // 벡터에서 뒤에서부터 빼면서 그 값이 끝나는 시간을 저장해둠, 그리고 while로 마지막 값의 시작시간이 저장한 끝나는 시간보다 작은지 확인하면서 계속뺌
    // 만약에 시작시간보다 더 큰게 나온다면 여기서 빼둔 친구들은 다 날리고 answer++, 그 다음에 더 큰시간 부터 다시 끝나는시간을 저장함
    
    while (!routes.empty()){
        int start = routes.back()[0];
        int end = routes.back()[1];
        routes.pop_back();
        while (!routes.empty()){
            if (routes.back()[0] <= end){
                end = min(end, routes.back()[1]);
                routes.pop_back();
            } else break;
        }
        answer++;
    }

    
    return answer;
}