#include <string>
#include <vector>
#include <iostream>
using namespace std;

vector<int> solution(vector<int> sequence, int k) {
    vector<int> answer(2, 0);
    int answer_len = 1000001;
    
    // 부분 수열의 합 찾기
    // 투포인터
    
    int start = 0;
    int sumV = 0;
    
    for (int end = 0; end < sequence.size(); end++){
        sumV += sequence[end];
        
        while (sumV > k && start < end){
            sumV -= sequence[start++];
        }
        
        if (sumV == k){
            if (answer_len > end - start + 1){
                answer_len = end - start + 1;
                answer[0] = start;
                answer[1] = end;
            } 
        }
    }
    
    return answer;
}