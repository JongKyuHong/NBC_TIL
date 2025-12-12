#include <iostream>
#include <string>
#include <algorithm>
using namespace std;

int main() {
    cin.tie(NULL);
    ios::sync_with_stdio(false);
    
    int n, m;
    cin >> n >> m;
    
    string board[50];
    for (int i = 0; i < n; i++) {
        cin >> board[i];
    }
    
    // 두 가지 체스판 패턴
    string pattern_b[8] = {
        "BWBWBWBW",
        "WBWBWBWB",
        "BWBWBWBW",
        "WBWBWBWB",
        "BWBWBWBW",
        "WBWBWBWB",
        "BWBWBWBW",
        "WBWBWBWB"
    };
    
    string pattern_w[8] = {
        "WBWBWBWB",
        "BWBWBWBW",
        "WBWBWBWB",
        "BWBWBWBW",
        "WBWBWBWB",
        "BWBWBWBW",
        "WBWBWBWB",
        "BWBWBWBW"
    };
    
    int min_count = 64;  // 최악의 경우 64개 모두 다시 칠함
    
    // 가능한 모든 8x8 영역 탐색
    for (int i = 0; i <= n - 8; i++) {
        for (int j = 0; j <= m - 8; j++) {
            int count_b = 0;  // B로 시작하는 패턴과 다른 칸의 개수
            int count_w = 0;  // W로 시작하는 패턴과 다른 칸의 개수
            
            // 8x8 영역 검사
            for (int x = 0; x < 8; x++) {
                for (int y = 0; y < 8; y++) {
                    if (board[i + x][j + y] != pattern_b[x][y]) {
                        count_b++;
                    }
                    if (board[i + x][j + y] != pattern_w[x][y]) {
                        count_w++;
                    }
                }
            }
            
            // 두 패턴 중 최소값
            int current_min = min(count_b, count_w);
            min_count = min(min_count, current_min);
        }
    }
    
    cout << min_count;
    
    return 0;
}
