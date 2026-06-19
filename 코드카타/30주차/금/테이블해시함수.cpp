// https://school.programmers.co.kr/learn/courses/30/lessons/147354

#include <string>
#include <vector>
#include <algorithm>
#include <iostream>

using namespace std;

int solution(vector<vector<int>> data, int col, int row_begin, int row_end) {
    int answer = -1;
    
    // 우선 col기준으로 정렬함
    // row_begin <= i <= row_end인 값에 대해서 S_i를 누적해서  bitwise XOR한 값을 해시 값으로 반환
    
    sort(data.begin(), data.end(), [col](auto& a, auto& b){
        if (a[col-1] != b[col-1]){
            return a[col-1] < b[col-1];
        } else {
            if (a[0] != b[0]){
                return a[0] > b[0];
            }
        }
    });
    
    // 우선 정렬 잘됨
    for (int i = row_begin; i<= row_end; i++){
        int accum = 0;
        for (int value : data[i-1]){
            accum += value % i;
        }
        if (answer == -1){
            answer = accum;
        } else {
            answer = answer ^ accum;
        }
    }
    
    
    return answer;
}