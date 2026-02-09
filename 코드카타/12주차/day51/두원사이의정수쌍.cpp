#include <string>
#include <vector>
#include <cmath>
#include <iostream>

using namespace std;

long long solution(int r1, int r2) {
    long long answer = 0;

    long long cnt = 0;
    
    for (int x = 1; x <= r2; x++){
        // 원의 경계 안쪽으로 들어와야 해서 floor
        long long maxY = floor(sqrt((long long)r2 * r2 - (long long)x * x));
        
        // r1원의 경계 밖으로 나와야 해서 ceil
        long long minY = 0;
        if (x < r1){
            minY = ceil(sqrt((long long)r1 * r1 - (long long)x * x));
        }
        
        answer += maxY - minY + 1;
    }
    return answer*4;
}