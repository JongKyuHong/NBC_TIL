// https://school.programmers.co.kr/learn/courses/30/lessons/468377

#include <string>
#include <vector>
#include <climits>
#include <algorithm>
#include <map>

using namespace std;

int answer = 0;
int N = 0;

void dfs(int depth, vector<vector<int>>& cost, vector<vector<int>>& hint, int total, map<int, int> bundle){
    if (depth == N) {
        answer = min(answer, total);
        return;
    }
    
    if (total >= answer) return;
    
    // bundle[depth]가 없더라도 0으로 생성되는데 그래도 상관없다고 생각
    int bundleCount = bundle[depth];
    if (bundleCount >= cost[depth].size()){
        bundleCount = cost[depth].size()-1;
    }
    int tmpCost = cost[depth][bundleCount];
    
    // 힌트 번들 구매 안할때
    dfs(depth+1, cost, hint, total + tmpCost, bundle);
    
    if (depth < N-1){
        // 구매하는 버전
        int hintCost = hint[depth][0];
        for (int i = 1; i < hint[depth].size(); ++i){
            // zero_index
            bundle[hint[depth][i]-1]++;
        }
        dfs(depth+1, cost, hint, total + tmpCost + hintCost, bundle);
    }
}

int solution(vector<vector<int>> cost, vector<vector<int>> hint) {
    answer = INT_MAX;
    N = cost.size();
    // cost에 힌트권 사용수에 따른 해결 비용
    // hint는 각 스테이지에서 구매 가능한 힌트 번들의 비용
    // dfs
    map<int, int> bundle;
    dfs(0, cost, hint, 0, bundle);

    return answer;
}