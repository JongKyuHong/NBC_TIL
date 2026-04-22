// https://school.programmers.co.kr/learn/courses/30/lessons/42889?language=cpp

#include <string>
#include <vector>
#include <algorithm>
#include <map>
#include <iostream>

using namespace std;

vector<int> solution(int N, vector<int> stages) {
    vector<pair<int,double>> answer;
    
    // 실패율이 높은 스테이지부터 출력, 실패율이 같다면 스테이지 번호가 작은순으로 출력하면 됨
    // N+1은 모든 스테이지 다 깬거
    // stages에 있는 숫자가 실패한 스테이지라는것과 같다 (현재 도전중인 스테이지)
    
    // 우선 순서는 상관없으므로 정렬
    sort(stages.begin(), stages.end());
    
    map<int, int> arrive;
    
    // 몇명이 멈춰있는지
    for (int stage : stages){
        arrive[stage]++;
    }
    
    int pc = stages.size();
    for (int i = 1; i <= N; i++){
        if (pc > 0){
            double rate = (double)arrive[i] / pc;
            answer.push_back({i, rate});
            pc -= arrive[i];
        } else {
            answer.push_back({i, 0});
        }
    }
    
    sort(answer.begin(), answer.end(), [](const auto& a, const auto& b){
        if (a.second != b.second){
            return a.second > b.second;
        }
        return a.first < b.first;
    });

    vector<int> result;
    for (auto a : answer){
        result.push_back(a.first);
    }
    
    return result;
}