#include <string>
#include <vector>
#include <sstream>
#include <iostream>
#include <unordered_map>

using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(NULL);

    int answer = 0;
    unordered_map<string, bool> isInfection;

    int N;
    cin >> N;
    bool flag = false;

    for (int i = 0; i < N; i++) {
        string name1;
        string name2;
        cin >> name1 >> name2;

        if (flag) {
            if (isInfection[name1] || isInfection[name2]) {
                isInfection[name1] = true;
                isInfection[name2] = true;
            }
        } else {
            if (name1 == "ChongChong" || name2 == "ChongChong") {
                isInfection[name1] = true;
                isInfection[name2] = true;
                flag = true;
            }
        }
    }

    for (auto& Infe : isInfection) {
        if (Infe.second) answer++;
    }

    cout << answer;

    return 0;
}
