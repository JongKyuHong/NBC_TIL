// https://school.programmers.co.kr/learn/courses/30/lessons/12911?language=cpp

#include <string>
#include <vector>
#include <algorithm>
#include <iostream>

using namespace std;

string convertToBinary(int a){
    string tmp = "";
    
    while (a > 0){
        tmp = (a % 2 == 0 ? '0' : '1') + tmp;
        a /= 2;
    }
    return tmp;
}

int solution(int n) {
    int answer = 0;
    
    string target = convertToBinary(n);
    
    int countOne = count(target.begin(), target.end(), '1');
    
    while (true){
        target = convertToBinary(++n);
        int tmp = count(target.begin(), target.end(), '1');
        if (tmp == countOne) {
            answer = n;
            break;
        }
    }
    
    return answer;
}