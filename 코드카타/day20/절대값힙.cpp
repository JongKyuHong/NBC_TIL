#include <iostream>
#include <queue>
#include <map>

int main() {
   std::ios::sync_with_stdio(false);
   std::cin.tie(NULL);

   std::priority_queue<int> pq;
   std::map<int, int> mm;

   int N;
   std::cin >> N;

   for (int i = 0; i < N; i++) {
       int x;
       std::cin >> x;

       int absx = std::abs(x);
       if (absx == 0) {
           if (pq.empty()) {
               std::cout << 0 << "\n";
               continue;
           }
           int target = -pq.top();
           // 같은 절대값이면 음수부터 빠지기 때문에
           if (mm.find(-target) != mm.end()) {
               std::cout << -target << "\n";
               mm[-target]--;
               if (mm[-target] == 0) {
                   mm.erase(-target);
               }
           } else {
               std::cout << target << "\n";
               mm[target]--;
               if (mm[target] == 0) {
                   mm.erase(target);
               }
           }
           pq.pop();
       } else {
           pq.push(-absx);
           mm[x]++;
       }
   }

   return 0;
}