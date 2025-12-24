#include <iostream>
#include <queue>

int main() {
   std::ios::sync_with_stdio(false);
   std::cin.tie(NULL);

   std::priority_queue<int> pq;
   int N;
   std::cin >> N;
   for (int i = 0; i < N; i++) {
       int x;
       std::cin >> x;
       if (x > 0) {
           pq.push(-x);
       } else {
           if (pq.empty()) {
               std::cout << 0 << "\n";
               continue;
           }
           std::cout << -pq.top() << "\n";
           pq.pop();
       }
   }

   return 0;
}