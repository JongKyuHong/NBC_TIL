#include <string>
#include <vector>
#include <climits>
#include <algorithm>
#include <cmath>

using namespace std;

vector<int> solution(int m, int n, int startX, int startY, vector<vector<int>> balls) {
    vector<int> answer;
    
    // 최소거리니까 4개면중 가장 가까운 면에 닿고나서 리턴되는것이 가장 가깝다.
    for (auto ball : balls){
        int x = ball[0];
        int y = ball[1];
        int minV = INT_MAX;
        // 왼쪽 아래 오른쪽 위라고 가정
        // 처음에는 그냥 계산했는데 부동소수점때매 애매하게 오차가 난다.
        // 해당하는 벽의 반대편으로 목표지점을 보내서 직선의 거리를 구하자
        for (int i = 0; i < 4; i++){
            if (i == 0){
                if (y == startY && x < startX){
                    continue;
                }
                int result = pow(x+startX, 2) + pow(abs(y-startY), 2);
                minV = min(minV, result);
            } else if (i == 1){
                if (x == startX && y < startY){
                    continue;
                }
                int result = pow(abs(x-startX), 2) + pow(y+startY, 2);
                minV = min(minV, result);
            } else if (i == 2){
                if (y == startY && x > startX){
                    continue;
                }
                int result = pow(2*m-x-startX, 2) + pow(abs(y-startY), 2);
                minV = min(minV, result);
            } else {
                if (x == startX && y > startY){
                    continue;
                }
                int result = pow(abs(x-startX), 2) + pow(2*n-y-startY, 2);
                minV = min(minV, result);
            }
        }
        answer.push_back(minV);
    }
    
    return answer;
}