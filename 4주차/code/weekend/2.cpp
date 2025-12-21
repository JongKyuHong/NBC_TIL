#include <iostream>
#include <vector>

int main() {
   std::ios::sync_with_stdio(false);
   std::cin.tie(NULL);

   int M, N;
   std::cin >> M >> N;

   std::vector<bool> isPrime(N + 1, true);
   isPrime[0] = isPrime[1] = false;

   for (int i = 2; i <= N; i++) {
       if (isPrime[i]) {
           for (int j = i*2; j <= N; j += i) {
               isPrime[j] = false;
           }
       }
   }

   for (int i = M; i <= N; i++) {
       if (isPrime[i]) {
           std::cout << i << "\n";
       }
   }
   

   return 0;
}