// https://school.programmers.co.kr/learn/courses/30/lessons/12921?language=cpp

#include <string>
#include <vector>

using namespace std;

int solution(int n) {
    int answer = 0;
    vector<bool> isPrime(n+1, true);
    isPrime[0] = false;
    isPrime[1] = false;
    vector<int> result;
    
    for (int i = 2; i <= n; i++){
        if (isPrime[i]) result.push_back(i);
        for (int j = i; j <= n; j += i){
            if (i == j) continue;
            isPrime[j] = false;
        }
    }
    answer = result.size();
    return answer;
}