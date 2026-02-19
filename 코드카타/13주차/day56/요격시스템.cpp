#include <string>
#include <algorithm>
#include <vector>

using namespace std;

int solution(vector<vector<int>> targets) {
    int answer = 0;
    
    // 시작시간 기준으로 어떤 벡터같은데다가 넣음
    // 맨 처음에 들어온 값 혹은 가장 끝나는시간이 짧은 애가 끝날때 result+=1 
    // 어디다가 저장??

    // 끝나는 시간 기준으로 정렬
    sort(targets.begin(), targets.end(), [](auto a, auto b){
       return a[1] < b[1];
    });
    
    int location = 0;

    for (const auto& target : targets){
        int start = target[0];
        int end = target[1];
        
        if (location <= start){
            answer++;
            location = end;
        }
    }
    
    
    return answer;
}