#include <iostream>
#include <vector>
#include <algorithm>
#include <string>

using namespace std;
int findMinValue(vector<string> vec, int X, int Y) {

    int bStart = 0;
    char pattern[] = {'B', 'W'};
    for (int i = X; i < 8 + X; i++) {
        char target = pattern[i % 2];
        for (int j = Y; j < Y+8; j++) {
            if (target != vec[i][j]) {
                bStart++;
            }
            if (target == 'B') {
                target = 'W';
            }
            else {
                target = 'B';
            }
            
        }
    }

    int wStart = 0;
    char pattern2[] = { 'W', 'B' };
    for (int i = X; i < 8 + X; i++) {
        char target = pattern2[i % 2];
        for (int j = Y; j < Y + 8; j++) {
            if (target != vec[i][j]) {
                wStart++;
            }
            if (target == 'B') {
                target = 'W';
            }
            else {
                target = 'B';
            }

        }
    }

    return min(wStart, bStart);
}

int main() {
    cin.tie(NULL);
    ios::sync_with_stdio(false);
    
    int N, M;
    cin >> N >> M;

    vector<string> vec(N);

    for (int i = 0; i < N; i++) {
        string inp;
        cin >> inp;
        vec[i] = inp;
    }

    int Bres = 50*50 + 1;
    
    for (int i = 0; i <= N - 8; i++) {
        for (int j = 0; j <= M - 8; j++) {
            Bres = min(Bres, findMinValue(vec, i, j));
        }
    }

    cout << Bres;
    return 0;
}
