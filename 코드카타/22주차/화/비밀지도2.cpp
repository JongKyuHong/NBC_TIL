// https://school.programmers.co.kr/learn/courses/30/lessons/17681?language=cpp

#include <string>
#include <vector>
#include <iostream>

using namespace std;

vector<string> solution(int n, vector<int> arr1, vector<int> arr2) {
    vector<string> answer;
    
    // 각행의 값을 이진수로 변환, 이진수의 길이는 n
    // 0이면 공백 1이면 벽 >> 둘다 1이면 벽으로 쳐준다. 비트연산자로 하면 편하게 할 수 있을거같긴함
    
    for (int i = 0; i < n; i++){
        int combined = arr1[i] | arr2[i];
        string result = "";
        for (int j = 0; j < n; j++) {
            if (combined % 2 == 1) result = "#" + result;
            else result = " " + result;
            combined /= 2;
        }
        answer.push_back(result);
    }
    
    return answer;
}