#include <string>
#include <vector>
#include <deque>
#include <algorithm>
#include <iostream>

using namespace std;

int solution(int n, vector<vector<int>> wires) {
    vector<int> answer;
    
    // 기준점
    for (int i = 0; i < wires.size(); i++){
        vector<vector<int>> vec(n);
        int tmp = -1;
        for (int j = 0; j < wires.size(); j++){
            if (i == j) continue;
            int n1 = wires[j][0];
            int n2= wires[j][1];
            vec[n1-1].push_back(n2-1);
            vec[n2-1].push_back(n1-1);
        }
        
        // 순회
        vector<bool> visited(n, false);
        for (int j = 0; j < n; j++){
            if (!visited[j]){
                deque<int> dq;
                dq.push_back(j);
                visited[j] = true;
                int count = 0;
                while (!dq.empty()){
                    int value = dq.front();
                    dq.pop_front();
                    count++;
                    for (int v : vec[value]){
                        if (!visited[v]){
                            dq.push_back(v);
                            visited[v] = true;
                        }
                    }
                }
                
                if (tmp == -1){
                    tmp = count;
                } else {
                    tmp = tmp - count;
                }
            }
        }    
        answer.push_back(abs(tmp));        
    }
    
    auto it = min_element(answer.begin(), answer.end());
    return *it;
}