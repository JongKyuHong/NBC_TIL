// https://school.programmers.co.kr/learn/courses/30/lessons/42626

#include <string>
#include <vector>
#include <iostream>
#include <queue>


using namespace std;

int solution(vector<int> scoville, int K) {
    int answer = 0;
    
    // 어떤 정렬이 들어가면 좋다. 
    // heap 직접 구현하기?
    priority_queue<int, vector<int>, greater<int>> pq;
    
    for (int i = 0; i < scoville.size(); i++){
        pq.push(scoville[i]);
    }
    
    while(pq.size() >= 2){
        int q = pq.top();
        if (q >= K) break;
        pq.pop();
        int q2 = pq.top();
        pq.pop();
        pq.push(q+(q2*2));
        answer++;
    }
    
    if (pq.top() < K) return -1;
    
    return answer;
}