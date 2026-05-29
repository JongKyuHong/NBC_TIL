// https://school.programmers.co.kr/learn/courses/30/lessons/17687?language=cpp

#include <string>
#include <vector>
#include <algorithm>
#include <iostream>

using namespace std;

string convertToBase(int num, int base){
    if(num == 0) return "0";
    
    string result = "";
    string chars = "0123456789ABCDEF";
    
    while (num > 0){
        result += chars[num%base];
        num /= base;
    }
    
    reverse(result.begin(), result.end());
    
    return result;
}


string solution(int n, int t, int m, int p) {
    string answer = "";
    
    // 헷갈리니까 0 index로
    p--;
    
    int currentNumber = 0;
    string currentTarget = convertToBase(currentNumber, n);
    int currentTargetIndex = 0;
    
    int count = 0;
    // turn이 m(참가인원)과 같아지면 다시 0번째 인원부터
    int turn = 0;
    while (count < t){
        if (turn == p){
            answer += currentTarget[currentTargetIndex];
            count++;
        }
        currentTargetIndex++;
        if (currentTargetIndex >= currentTarget.length()){
            currentTarget = convertToBase(++currentNumber, n);
            currentTargetIndex = 0;
        }
        turn++;
        
        if (turn >= m){
            turn = 0;
        }
    }
    
    
    return answer;
}