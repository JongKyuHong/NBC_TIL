#include <iostream>
#include <stack>

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(NULL);

    int n;
    std::cin >> n;

    std::stack<int> stack_;
    int target = 1;
    bool flag = true;
    std::string result = "";
    for (int i = 0; i < n; i++) {
        int inp;
        std::cin >> inp;

        if (inp >= target) {
            while (inp >= target) {
                stack_.push(target);
                target++;
                result += "+";
            }
            if (stack_.top() == inp) {
                stack_.pop();
                result += "-";
            }
        } else {
            if (stack_.top() == inp) {
                stack_.pop();
                result += "-";
            } else {
                flag = false;
                break;
            }
        }
    }

    if (!flag) {
        std::cout << "NO";
    } else {
        for (int i = 0; i < result.length(); i++) {
            std::cout << result[i] << "\n";
        }
    }

    return 0;
}