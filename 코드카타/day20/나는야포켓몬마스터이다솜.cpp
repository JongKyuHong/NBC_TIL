#include <iostream>
#include <unordered_map>
#include <string>

int main() {
   std::ios::sync_with_stdio(false);
   std::cin.tie(NULL);

   std::unordered_map<std::string, int> umap;
   std::unordered_map<int, std::string> umap2;

   int N, M;
   std::cin >> N >> M;

   for (int i = 1; i <= N; i++) {
       std::string inp;
       std::cin >> inp;
       umap.insert({ inp, i });
       umap2.insert({ i, inp });
   }

   for (int i = 0; i < M; i++) {
       std::string inp;
       std::cin >> inp;
       char tmp = inp[0];
       if (tmp - '0' >= 0 && tmp - '0' <= 9) {
           std::cout << umap2[stoi(inp)] << "\n";
       } else {
           std::cout << umap[inp] << "\n";
       }
   }

   return 0;
}