#include <iostream>
#include <string>
#include <sstream>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(NULL);
    int T;
    cin >> T;
    cin.ignore();
    for (int i = 0; i < T; i++) {
        int sumV = 0;
        string inp;
        getline(cin, inp);
        stringstream ss(inp);
        int num = 0;

        while (ss >> num) {
            sumV += num;
        }
        
        cout << sumV << "\n";
    }
    return 0;
}