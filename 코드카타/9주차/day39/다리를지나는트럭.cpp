#include <string>
#include <vector>
#include <queue>
#include <iostream>

using namespace std;

int solution(int bridge_length, int weight, vector<int> truck_weights) {
    queue<int> q;
    
    for (int i = 0; i < bridge_length; i++){
        q.push(0);
    }
    
    int total_weight = 0;
    int index = 0;
    int time = 0;
    
    while (index < truck_weights.size()){
        time++;
        int value = q.front();
        if (value > 0){
            total_weight -= value;
        }
        q.pop();
        
        if (total_weight+truck_weights[index] <= weight){
            q.push(truck_weights[index]);
            total_weight += truck_weights[index];
            index++;
        } else {
            q.push(0);
        }    
    }
    
    time += bridge_length;
    
    return time;
}