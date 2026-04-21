// https://school.programmers.co.kr/learn/courses/30/lessons/17681?language=cpp

#include <string>
#include <vector>
#include <iostream>

using namespace std;

string makeBinary(int n, int value){
    string result = "";
    while (value > 0){
        int tmp = value % 2;
        result = to_string(tmp) + result;
        value /= 2;
    }
    
    while (result.length() < n){
        result = '0' + result;
    }
    
    return result;
}

vector<string> solution(int n, vector<int> arr1, vector<int> arr2) {
    vector<string> answer;
    
    // 각행의 값을 이진수로 변환, 이진수의 길이는 n
    // 0이면 공백 1이면 벽 >> 둘다 1이면 벽으로 쳐준다. 비트연산자로 하면 편하게 할 수 있을거같긴함
    
    for (int i = 0; i < n; i++){
        // 둘다 변환?
        string arr1BinaryStr = makeBinary(n, arr1[i]);
        string arr2BinaryStr = makeBinary(n, arr2[i]);
        string result = "";
        // cout << "arr1 : " << arr1BinaryStr << "\n" << "arr2 : " << arr2BinaryStr << "\n";
        for (int j = 0; j < n; j++){
            if (arr1BinaryStr[j] == '0' && arr2BinaryStr[j] == '0'){
                result += ' ';
            } else {
                result += '#';
            }
        }
        answer.push_back(result);
    }
    
    return answer;
}