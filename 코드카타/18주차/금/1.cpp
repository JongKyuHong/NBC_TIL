#include <iostream>

int main() {
    int n1, n2, n12;
    std::cin >> n1 >> n2 >> n12;
    std::cout << (n1 + 1) * (n2 + 1) / (n12 + 1) - 1 << std::endl;
    return 0;
}