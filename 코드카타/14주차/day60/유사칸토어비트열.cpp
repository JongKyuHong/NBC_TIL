#include <vector>
#include <numeric>
#include <cmath>

using namespace std;

long long countOnes(int n, long long k) {
    if (n == 0) return 1;
    if (k < 0) return 0;
    
    long long len = pow(5, n-1);
    long long countOne = pow(4, n-1);
    
    int sector = k / len;
    long long nums = k % len;
    
    if (sector == 0){
        return countOnes(n-1, nums);
    } else if (sector == 1){
        return countOne + countOnes(n-1, nums);
    } else if (sector == 2){
        return countOne * 2;
    } else if (sector == 3){
        return countOne * 2 + countOnes(n-1, nums);
    } else if (sector == 4){
        return countOne * 3 + countOnes(n-1, nums);
    }
}

int solution(int n, long long l, long long r) {
    return countOnes(n, r - 1) - countOnes(n, l - 2);
}