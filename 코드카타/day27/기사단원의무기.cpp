#include <string>
#include <vector>

using namespace std;

int solution(int number, int limit, int power) {
    vector<int> CountPrime(number+1, 0);
    
    for (int i = 1; i <= number; i++){
        for (int j = i; j <= number; j+=i){
            CountPrime[j]++;
        }
    }
    
    int answer = 0;
    for (int i = 1; i <= number; i++){
        if (CountPrime[i] > limit){
            answer += power;
        } else {
            answer += CountPrime[i];
        }
    }
    
    return answer;
}