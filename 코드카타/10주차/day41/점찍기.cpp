#include <string>
#include <vector>
#include <cmath>
#include <iostream>

using namespace std;

long long solution(int k, int d) {
    long long answer = 0;
    
    for (int i = 0; i <= d; i += k){
        long long tmp = sqrt(pow(d,2) - pow(i ,2));
        answer += tmp / k;
        answer++;
    }
    
    return answer;
}