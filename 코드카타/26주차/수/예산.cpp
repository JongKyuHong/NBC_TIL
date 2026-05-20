// https://school.programmers.co.kr/learn/courses/30/lessons/12982?language=cpp

#include <iostream>
#include <stdio.h>
#include <string>
#include <vector>
#include <algorithm>

using namespace std;

int solution(vector<int> d, int budget) {
    int answer = 0;
    sort(d.begin(), d.end());
    for (int i : d){
        if (i <= budget){
            budget -= i;
            answer++;
        }
    }
    
    return answer;
}