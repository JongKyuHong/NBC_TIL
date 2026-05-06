// https://school.programmers.co.kr/learn/courses/30/lessons/468373

#include <string>
#include <vector>
#include <algorithm>
#include <iostream>

using namespace std;

int NodeCnt = 0;
int EdgeCnt = 0;
int maxV = 0;

void dfs(int pipeType, vector<bool> canVisitedNodes, vector<bool> canVisitedEdges, vector<vector<int>> edges, int cnt){
    if (cnt == 0) {
        int tmp = 0;
        for (int i = 1; i <= NodeCnt; ++i){
            if (canVisitedNodes[i]) tmp++;
        }
        maxV = max(maxV, tmp);
        return;
    }
    
    bool flag = true;
    do {
        flag = false;
        // 현재 가진 노드에서 인접한 간선 추가
        for (int i = 0; i < EdgeCnt; ++i){
            if (canVisitedEdges[i]) continue;
            int x = edges[i][0];
            int y = edges[i][1];
            int pType = edges[i][2];
            if (canVisitedNodes[x] || canVisitedNodes[y]) {
                canVisitedEdges[i] = true;
                flag = true;
            }
        }
    
        // pType에 맞게 가진 간선들을 연다. 노드들 감염, 만약 감염시킨 노드에 연결된 간선들도 pType이라면 연쇄감염시켜야 함
        for (int i = 0; i < EdgeCnt; ++i){
            if (canVisitedEdges[i]){
                int x = edges[i][0];
                int y = edges[i][1];
                int pType = edges[i][2];
                if (pipeType == pType){
                    // 둘다그냥 true로 계속 초기화해버리자
                    if (canVisitedNodes[x] && canVisitedNodes[y]) continue;
                    canVisitedNodes[x] = true;
                    canVisitedNodes[y] = true;
                    flag = true;
                }
            }
        }
    } while (flag);
    
    
    for (int i = 1; i <= 3; ++i){
        dfs(i, canVisitedNodes, canVisitedEdges, edges, cnt-1);   
    }
}

int solution(int n, int infection, vector<vector<int>> edges, int k) {
    // 노드도 따로 저장하고
    // edges도 감염된다고 생각하고 써야될듯 그래야 편함
    NodeCnt = n;
    EdgeCnt = edges.size();
    vector<bool> canVisitedNodes(NodeCnt+1);
    vector<bool> canVisitedEdges(EdgeCnt);
    canVisitedNodes[infection] = true;
    
    for (int i = 1; i <= 3; ++i){
        dfs(i, canVisitedNodes, canVisitedEdges, edges, k);  
    }

    return maxV;
}