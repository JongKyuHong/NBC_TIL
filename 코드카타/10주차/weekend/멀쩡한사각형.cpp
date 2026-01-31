#include <numeric>

using namespace std;

long long solution(int w,int h) {
    long long answer = 1;
    
    long long N = gcd(w, h);

    long long newW = w / N;
    long long newH = h / N;
    
    // 얘네 둘의 패턴에서 사라지는 상자가 몇개인지 확인한다.
    // 처음시작 하나 w-1개 가로로 부수고 h-1개 세로로부숨
    long long tmp = N * (newW + newH - 1);
    long long tmp2 = (long long)w*(long long)h; 
    answer = tmp2 - tmp;
    return  answer;
}