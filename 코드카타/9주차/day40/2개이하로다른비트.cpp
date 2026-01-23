#include <string>
#include <vector>
#include <iostream>

using namespace std;

string convertBinary(long long number){
    string tmp = "";
    while (number > 0){
        tmp = to_string(number%2) + tmp ;
        number /= 2;
    }
    return tmp;
}

long long find(string& result){
    bool flag = true;
    for (int i = result.length() - 1; i >= 0; i--){
        if (result[i] == '0'){
            // 이부분을 1로바꾸고
            // 바로뒤 1을 0으로 바꾸면 최솟값
            result[i] = '1';
            if (i + 1 < result.length()){
                result[i+1] = '0';
            }
            flag = false;
            break;
        }
    }
    
    if (flag){
        result = '1' + result;
        result[1] = '0';    
    }

    long long res = stoll(result, nullptr, 2);
    
    return res;
}

vector<long long> solution(vector<long long> numbers) {
    vector<long long> answer;
    
    for (long long number : numbers){
        if (number == 0){
            answer.push_back(1);
            continue;
        }
        string result = convertBinary(number);
        answer.push_back(find(result));
    }
    
    return answer;
}