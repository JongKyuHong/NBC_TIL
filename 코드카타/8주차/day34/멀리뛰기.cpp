#include <string>
#include <vector>

using namespace std;

long long solution(int n) {
    vector<long long> vec(n+1, 0);
    
    vec[1] = 1;
    vec[2] = 2;

    for (int i = 3; i <= n; i++){
        vec[i] = (vec[i-1] + vec[i-2])%1234567;
    }
    
    return vec[n];
}