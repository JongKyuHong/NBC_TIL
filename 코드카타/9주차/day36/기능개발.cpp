#include <string>
#include <vector>
#include <queue>

using namespace std;

vector<int> solution(vector<int> progresses, vector<int> speeds) {
    vector<int> answer;
    queue<int> q;
    // 얼마나 걸리는지를 미리계산하고, 그 계산값을 큐에 넣음
    // 큐에서 나올때 해당 값보다 작은 애들을 모두 꺼냄
    
    int len = progresses.size();
    
    for (int i = 0; i < len; i++){
        int cnt = 1;
        while (progresses[i] + speeds[i]*cnt < 100){
            cnt++;
        }
        // 여기서 cnt가 걸리는 일수임 이걸 큐에넣음
        q.push(cnt);
    }
    
    while (!q.empty()){
        int val = q.front();
        q.pop();
        int cnt = 1;
        while (!q.empty()){
            if (val >= q.front()){
                q.pop();
                cnt++;
            } else {
                break;
            }
        }
        answer.push_back(cnt);
    }
    
    
    return answer;
}