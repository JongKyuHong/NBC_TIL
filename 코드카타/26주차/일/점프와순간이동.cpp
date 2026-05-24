// https://school.programmers.co.kr/learn/courses/30/lessons/12980?language=cpp

#include <iostream>
using namespace std;

int solution(int n)
{
    int ans = 0;
                
    while (n != 0){
        if (n % 2){
            n -= 1;
            ans++;
        }
        n /=2;
    }

    return ans;
}