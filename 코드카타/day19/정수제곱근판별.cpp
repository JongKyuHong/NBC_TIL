#include <string>
#include <vector>
#include <cmath>

using namespace std;

long long solution(long long n) {
    int tmp = int(sqrt(n));
    if (n == pow(tmp, 2)){
        return pow(tmp+1, 2);
    } else {
        return -1;
    }
}