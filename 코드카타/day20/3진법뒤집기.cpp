#include <string>
#include <vector>
#include <iostream>
#include <cmath>

using namespace std;

// 3진법 변환하면서 애초에 반대로 출력해줌
string convert(int num){
    string result = "";
    while (num > 0){
        int mod = num % 3;
        result = result + to_string(mod);
        num /= 3;
    }
    return result;
}


int solution(int n) {
    int answer = 0;
    
    string result = convert(n);
    int cnt = 0;
    for (int i = result.length() - 1; i >= 0; i--){
        answer += (result[i]-'0') * pow(3, cnt++);
    }
    
    return answer;
}