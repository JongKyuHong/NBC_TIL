#include <iostream>
#include <algorithm>
#include <vector>
#include <string>

using namespace std;

int N, M;

void parser(int row, int col, vector<string>& vec) {
    char num1 = vec[row][col+1];
    char num2 = vec[row][col+3];

    string result = "";
    // 결과가 한자리면 5번에만, 두자리면 5번6번
    char tmp = vec[row][col+6];
    if (tmp != '.') {
        result = string(1,vec[row][col+5]) + vec[row][col+6];
    } else {
        result += vec[row][col+5];
    }
    int n1 = num1 - '0';
    int n2 = num2 - '0';
    if (n1 + n2 == stoi(result)) {
        // 정답
        vec[row][col] = '*';
        if (result.length() == 1) {
            vec[row][col+6] = '*';
            for (int i = 1; i <= 5; i++) {
                vec[row - 1][col + i] = '*';
                vec[row + 1][col + i] = '*';
            }
        } else {
            vec[row][col+7] = '*';
            for (int i = 1; i <= 6; i++) {
                vec[row - 1][col + i] = '*';
                vec[row + 1][col + i] = '*';
            }
        }
        
    } else {
        // 오답
        vec[row - 1][col+3] = '/';
        vec[row][col+2] = '/';
        vec[row + 1][col+1] = '/';
    }
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(NULL);
    cin >> N >> M;
    vector<string> vec(3*N, "");

    for (int i = 0; i < 3 * N; i++) {
        for (int j = 0; j < 8 * M; j++) {
            char inp;
            cin >> inp;
            vec[i].push_back(inp);
        }
    }

    int row = 1;
    int col = 0;

    // 문자열 파싱
    while (true) {
        parser(row, col, vec);
        if (col + 8 < 8 * M) {
            col += 8;
        } else {
            col = 0;
            row += 3;
            if (row >= 3 * N) {
                break;
            }
        }
    }

    for (auto ve : vec) {
        for (auto v : ve) {
            cout << v;
        }
        cout << "\n";
    }

    return 0;
}