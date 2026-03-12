#include <iostream>
#include <string>
#include <numeric>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(NULL);

    long long A, B;
    cin >> A >> B;
    
    long long answer = gcd(A, B);
    
    string tmp = string(answer, '1');
    
    cout << tmp;
    
    return 0;
}