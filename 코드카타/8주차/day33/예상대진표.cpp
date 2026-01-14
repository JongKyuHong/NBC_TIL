#include <iostream>

using namespace std;

int solution(int n, int a, int b)
{
    int answer = 0;

    // 짝수면 그냥 나눔
    // 홀수면 +1하고 2로나눔
    
    while (true){
        answer++;
        int tmpA = a;
        int tmpB = b;
        if (a % 2){
            tmpA++;
        }
        if (b % 2){
            tmpB++;
        }
        if (tmpA / 2 == tmpB / 2){
            return answer;
        }
        if (a >= 2){
            if (a % 2){
                a++;
            }
            a /= 2;
        }
        if (b >= 2){
            if (b % 2){
                b++;
            }
            b/=2;
        }
    }
}