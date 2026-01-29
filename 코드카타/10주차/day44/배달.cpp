#include <iostream>
#include <vector>
#include <queue>
#include <iostream>

using namespace std;

void dijkstra(vector<vector<pair<int, int>>>& vec, vector<int>& dist){
    priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> pq;
    dist[1] = 0;
    pq.push({0, 1});
    while (!pq.empty()){
        int d = pq.top().first;
        int v = pq.top().second;
        pq.pop();
        
        if (dist[v] < d) continue;
        
        for (auto& node : vec[v]){
            int next = node.first;
            int nextDist = node.second;
            
            int newDist = d+nextDist;
            if (newDist < dist[next]){
                dist[next] = newDist;
                pq.push({newDist, next});
            }
        }
    }
    
}

int solution(int N, vector<vector<int> > road, int K) {
    int answer = 0;

    vector<vector<pair<int, int>>> vec(N+1);
    
    for (const auto& r : road){
        int start = r[0];
        int end = r[1];
        int d = r[2];
        
        vec[start].push_back({end, d});
        vec[end].push_back({start, d});
    }
    
    vector<int> dist(N+1, 500001);
    
    dijkstra(vec, dist);
    
    for (int i = 1; i < dist.size(); i++){
        if (dist[i] <= K){
            // cout << i << endl;
            answer++;
        }
    }

    return answer;
}