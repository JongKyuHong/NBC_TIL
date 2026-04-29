// https://school.programmers.co.kr/learn/courses/30/lessons/250125

#include <string>
#include <vector>

using namespace std;

int solution(vector<vector<string>> board, int h, int w) {
    int answer = 0;
    int N = board.size();
    vector<pair<int, int>> delta = {{0,1},{0,-1},{-1,0},{1,0}};
    string standard = board[h][w];
    for (int i = 0; i < delta.size(); ++i){
        int nr = h + delta[i].first;
        int nc = w + delta[i].second;
        if (nr < N && nr >= 0 && nc < N && nc >= 0 && board[nr][nc] == standard) answer++;
    }
    
    return answer;
}