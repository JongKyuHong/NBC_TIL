#include <string>
#include <vector>
#include <queue>
#include <iostream>

using namespace std;

int solution(int n, int k, vector<int> enemy) {
    int answer = 0;
    
    priority_queue<int> pq;
    int remain = n;
    int i = 0;
    while (i < enemy.size()){
        pq.push(enemy[i]);
        remain -= enemy[i];
        if (remain < 0){
            if (k <= 0){
                break;
            }
            k -= 1;
            remain += pq.top();
            pq.pop();
        }
        i++;
    }
    
    return i;
}