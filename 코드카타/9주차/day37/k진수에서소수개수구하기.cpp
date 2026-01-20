#include <string>
#include <vector>
#include <cmath>
#include <iostream>

using namespace std;

string transform(int n, int k){
    string tmp = "";
    
    while (n > 0){
        int divide = n % k;
        tmp = to_string(divide) + tmp;
        n = n/k;
    }
    
    return tmp;
}

bool check(string value){
    long long tmp = stoll(value);
    if (tmp == 1) return false;    
    if (tmp <= 3) return true;
    
    for (long long i = 2; i <= sqrt(tmp); i++){
        if (tmp % i == 0){
            return false;
        }
    }
    return true;
}

int solution(int n, int k) {
    int answer = 0;
    
    string transformDigit = transform(n, k);
    // cout << transformDigit << endl;
    string value = "";
    for (int i = 0; i < transformDigit.size(); i++){
        if (transformDigit[i] == '0'){
            if (value.length()){
                if (check(value)) {
                    // cout <<  "value : " << value << endl;
                    answer++;
                }
                value = "";    
            }
        } else {
            value += transformDigit[i];
        }
    }
    
    if (value.length()){
        if (check(value)) {
            // cout <<  "value : " << value << endl;   
            answer++;
        }
    }
    
    return answer;
}