#include <string>
#include <vector>
#include <utility>
#include <queue>

using namespace std;

int solution(vector<int> priorities, int location) {
    int answer = 0;

    queue<pair<int, int>> q;

    priority_queue<int> pq;

    for (int i = 0; i < priorities.size(); i++) {
        q.push({i, priorities[i]});
        pq.push(priorities[i]);
    }

    while (!q.empty()) {
        int current_location = q.front().first;
        int current_priority = q.front().second;
        q.pop();

        if (current_priority == pq.top()) {
            answer++;
            pq.pop();
            if (current_location == location) {
                return answer;
            }
        } else {
            q.push({current_location, current_priority});
        }
    }

    return answer;
}