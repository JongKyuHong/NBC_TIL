#include <string>
#include <vector>
#include <string>
#include <algorithm>

using namespace std;

long long solution(long long n) {
    long long answer = 0;
    string newN = to_string(n);
    sort(newN.begin(), newN.end(), greater<char>());
    
    return stoll(newN);
}