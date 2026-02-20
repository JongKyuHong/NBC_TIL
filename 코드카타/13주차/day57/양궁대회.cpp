#include <string>
#include <vector>

using namespace std;
int maxV = 0;
int lowScore = -1;
vector<int> maxRes = {-1};
void dfs(int depth, vector<int>& info, vector<int>& res, int remain){
    if (depth == 11){
        if (remain > 0){
            res[10] = remain;
        }
        int rionScore = 0;
        int appeachScore = 0;
        int lowIndex = 0;
        for (int i = 0; i < 11; i++){
            if (info[i] < res[i]){
                rionScore += 10 - i;
                lowIndex = i;
            } else {
                if (info[i] > 0){
                    appeachScore += 10 - i;    
                }
            }
        }
        if (appeachScore >= rionScore){
            res[10] = 0;
            return;
        }
        int answer = rionScore - appeachScore;
        if (maxV < answer){
            maxV = answer;
            lowScore = lowIndex;
            maxRes = res;
        } else if (maxV == answer){
            if (lowScore < lowIndex){
                maxV = answer;
                lowScore = lowIndex;
                maxRes = res;
            }
        }
        res[10] = 0;
        return;
    }
    if (remain > info[depth]){
        res[depth] = info[depth]+1;
        dfs(depth+1, info, res, remain-info[depth]-1);
        res[depth] = 0;
    }
    dfs(depth+1, info, res, remain);
}

vector<int> solution(int n, vector<int> info) {
    vector<int> answer;

    // 어떤 알고리즘을 사용??
    // 1. 가장 높은 점수차
    // 2. 점수차가 같다면 가장 낮은 점수를 더 많이 맞힌 경우
    // 3. 가장 낮은 점수를 맞힌 개수가 같은경우 계속해서 그 다음 낮은 점수를 더 많이 맞힌 경우
    vector<int> res(11, 0);
    dfs(0, info, res, n);
    
    
    return maxRes;
}