#include <string>
#include <vector>
#include <unordered_map>
#include <string>
#include <iostream>

using namespace std;

long long solution(string s) {
    string answer = "";
    unordered_map<string, int> um({ {"zero", 0}, {"one", 1}, {"two", 2}, {"three",3},{"four",4},{"five",5},{"six", 6},{"seven",7},{"eight",8},{"nine", 9} });

    int index = 0;
    string current = "";
    while (index < s.length()) {
        if (s[index] - '0' >= 0 && s[index] - '0' <= 9) {
            answer += s[index];
        } else {
            current += s[index];
            if (current.length() >= 3) {
                if (um.find(current) != um.end()) {
                    answer += to_string(um[current]);
                    current = "";
                }
            }
        }
        index++;
    }

    long long result = stoi(answer);

    return result;
}