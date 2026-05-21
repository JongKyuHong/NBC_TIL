// https://school.programmers.co.kr/learn/courses/30/lessons/42861?language=cpp

#include <string>
#include <vector>
#include <algorithm>
#include <iostream>

using namespace std;

vector<int> parent;

int find_parent(int x){
    if (parent[x] != x){
        return find_parent(parent[x]);
    }    
    return parent[x];
}

void merge(int a, int b){
    a = find_parent(a);
    b = find_parent(b);
    
    if (a < b){
        parent[b] = a;
    } else {
        parent[a] = b;
    }
}

int solution(int n, vector<vector<int>> costs) {
    int answer = 0;
    
    // 무조건 n-1보장
    sort(costs.begin(), costs.end(), [](const auto& a, const auto& b){
        if (a[2] != b[2]) return a[2] < b[2];
    });
    
    parent.reserve(n);
    
    for (int i = 0; i < n; i++){
        parent[i] = i;
    }
    
    for (const vector<int>& cost : costs){
        int a = cost[0];
        int b =  cost[1];
        int value = cost[2];
        if (find_parent(a) != find_parent(b)){
            merge(a, b);
            answer += value;
        }
    }

    return answer;
}