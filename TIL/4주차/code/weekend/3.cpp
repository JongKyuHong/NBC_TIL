#include <iostream>
#include <queue>

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(NULL);

    int T;
    std::cin >> T;

    for (int _ = 0; _ < T; _++) {
        std::priority_queue<int> pq;
        std::queue<int> q;
        int N, M;
        std::cin >> N >> M;

        for (int i = 0; i < N; i++) {
            int inp;
            std::cin >> inp;
            pq.push(inp);
            if (i == M) {
                q.push(-inp);
            } else {
                q.push(inp);
            }
        }
        
        int cnt = 0;
        while (q.size()) {
            int target = q.front();
            if (target < 0) {
                if (pq.top() <= -target) {
                    cnt++;
                    break;
                } else {
                    q.pop();
                    q.push(target);
                }
            } else {
                if (pq.top() <= target) {
                    cnt++;
                    q.pop();
                    pq.pop();
                } else {
                    q.pop();
                    q.push(target);
                }
            }
        }

        std::cout << cnt <<"\n";
    }

    return 0;
}