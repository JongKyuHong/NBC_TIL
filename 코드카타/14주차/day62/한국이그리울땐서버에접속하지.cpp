#include <string>
#include <vector>
#include <algorithm>
#include <iostream>

using namespace std;

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);

    int N;
    cin >> N;

    string pattern;
    cin >> pattern;

    for (int i = 0; i < N; i++) {
        string tmp;
        cin >> tmp;
        int index = 0;
        // *을 만나기전에 다른게 생기면 NE
        bool flag = true;
        if (tmp.length() < pattern.length()-1) {
            cout << "NE" << "\n";
            continue;
        }
        int startIndex = 0;
        int endIndex = tmp.length() - 1;
        for (int j = startIndex; j < tmp.length(); j++) {
            if (pattern[index] == '*') break;
            if (tmp[j] != pattern[index]) {
                flag = false;
                break;
            }
            index++;
        }
        index = pattern.length() - 1;
        for (int j = endIndex; j >= 0; j--) {
            if (pattern[index] == '*') break;
            if (tmp[j] != pattern[index]) {
                flag = false;
                break;
            }
            index--;
        }
        if (flag) {
            cout << "DA" << "\n";
        }
        else {
            cout << "NE" << "\n";
        }
    }

    return 0;
}