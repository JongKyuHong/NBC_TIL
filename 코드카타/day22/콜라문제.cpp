#include <string>
#include <vector>

using namespace std;

int solution(int a, int b, int n) {
    int answer = 0;
    
    
    while (true){
        int div = n % a;
        int mok = n / a;
        if (mok > 0){
            answer += mok * b;
            n = mok*b + div;
        } else {
            break;
        }
    }
    

    
    return answer;
}