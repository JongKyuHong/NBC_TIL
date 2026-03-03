#include <string>
#include <vector>

using namespace std;

int getCol(int current, int w){
    int row = (current - 1) / w;
    // 해당 행이 홀수면 역방향으로 셈
    if (row % 2){
        return w - 1 - (current-1)%w;
    } else {
        return (current-1)%w;
    }
}

int solution(int n, int w, int num) {
    int answer = 0;
    int targetCol = getCol(num, w);
    
    for (int i = num; i <= n; i++){
        if (getCol(i, w) == targetCol) answer++;
    }
    
    return answer;
}