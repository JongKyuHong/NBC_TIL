#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

using namespace std;

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    string longestWord = "";
    string currentInput;

    while (cin >> currentInput) {
        if (currentInput == "E-N-D") break;

        string cleaned = "";
        for (char c : currentInput) {
            if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == '-') {
                cleaned += c;
            } else {
                if (cleaned.length() > longestWord.length()) {
                    longestWord = cleaned;
                }
                cleaned = "";
            }
        }

        if (cleaned.length() > longestWord.length()) {
            longestWord = cleaned;
        }
    }

    for (int i = 0; i < longestWord.length(); i++) {
        if (longestWord[i] >= 'A' && longestWord[i] <= 'Z') {
            cout << (char)(longestWord[i] + 32);
        } else {
            cout << longestWord[i];
        }
    }
    cout << endl;

    return 0;
}