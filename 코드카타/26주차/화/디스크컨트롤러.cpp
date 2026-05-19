// https://school.programmers.co.kr/learn/courses/30/lessons/42627?language=cpp

#include <string>
#include <vector>
#include <queue>
#include <algorithm>
#include <iostream>


using namespace std;

struct Compare{
    bool operator()(const pair<int, int>& a, const pair<int, int>& b){
        if (a.second != b.second) return a.second > b.second;
        return a.first > b.first;
    }
};

int solution(vector<vector<int>> jobs) {
    int answer = 0;
    
    // 작업 번호, 요청시각, 소요시간 저장하는 대기큐가 있음
    // 하드디스크가 놀고, 대기큐에 뭐가 있으면 가장 우선순위가 높은 작업을 시킴, 작업 소요시간짧은것 > 요청시각 빠른것 > 작업 번호 작은것
    // 한번 일시작하면 그 일만 함
    sort(jobs.begin(), jobs.end());
    priority_queue<pair<int,int>, vector<pair<int, int>>, Compare> pq;

    // 현시간
    int time = 0;
    int total = 0;
    int jobIdx = 0;
    int count = 0;
    while (count < jobs.size()){
        while (jobIdx < jobs.size() && jobs[jobIdx][0] <= time){
            pq.push({jobs[jobIdx][0], jobs[jobIdx][1]});
            jobIdx++;
        }
        
        if (!pq.empty()){
            total += (time + pq.top().second - pq.top().first);
            time += pq.top().second;
            pq.pop();
            count++;
        } else {
            time = jobs[jobIdx][0];
        }
    }
    
    answer = total / count;
    
    return answer;
}