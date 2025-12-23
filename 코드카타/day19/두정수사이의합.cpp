#include <string>
#include <vector>
#include <algorithm>

using namespace std;

long long solution(int a, int b) {
    long long answer = 0;
    int minV = min(a, b);
    int maxV = max(a, b);
    for (int i = minV; i <= maxV; i++){
        answer += i;
    }
    
    return answer;
}