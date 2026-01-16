#include <string>
#include <vector>
#include <sstream>
#include <iostream>
#include <unordered_map>

using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(NULL);
    int N;
    cin >> N;

    int i = 1;
    while (true) {
        if ((N * i) % 360 == 0) {
            cout << i;
            return 0;
        }
        i++;
    }
}
