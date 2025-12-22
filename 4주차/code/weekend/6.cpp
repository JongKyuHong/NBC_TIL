#include <iostream>
#include <unordered_map>

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(NULL);

    int M;
    std::cin >> M;

    std::unordered_map<int, int> mm;

    for (int i = 0; i < M; i++) {
        std::string inp;
        std::cin >> inp;

        if (inp == "all") {
            for (int i = 1; i <= 20; i++) {
                mm[i] = 1;
            }
        } else if (inp == "empty") {
            for (int i = 1; i <= 20; i++) {
                mm[i] = 0;
            }
        } else {
            int num;
            std::cin >> num;
            if (inp == "add") {
                mm[num] = 1;
            } else if (inp == "remove") {
                mm[num] = 0;
            } else if (inp == "check") {
                if (mm[num] == 1) {
                    std::cout << 1 << "\n";
                } else {
                    std::cout << 0 << "\n";
                }
            } else if ("toggle") {
                if (mm[num] == 0) {
                    mm[num] = 1;
                } else{
                    mm[num] = 0;
                }
            }
        }
    }

    return 0;
}