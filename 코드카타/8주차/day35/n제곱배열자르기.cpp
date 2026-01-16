#include <string>
#include <vector>

using namespace std;

vector<int> solution(int n, long long left, long long right) {
    vector<int> answer;
    for (long long i = left; i <= right; i++){
        long long quotient = i / n;
        long long remainder = i % n;
        
        if (quotient > remainder){
            long long tmp = remainder;
            remainder = quotient;
            quotient = tmp;
        }
        answer.push_back(remainder+1);
    }
    
    
    
    return answer;
}