#include <string>
#include <vector>

using namespace std;

bool check(vector<string>& board, char target){
    for (int i = 0; i < 3; i++) {
        if (board[i][0] == target && board[i][1] == target && board[i][2] == target) return true;
        if (board[0][i] == target && board[1][i] == target && board[2][i] == target) return true;
    }
    
    if (board[0][0] == target && board[1][1] == target && board[2][2] == target) return true;
    if (board[0][2] == target && board[1][1] == target && board[2][0] == target) return true;
    
    return false;
}


int solution(vector<string> board) {
    // 선공은 'O'
    // O랑 X의 개수가 같다 -> 가능, O가 1개 더 많다 가능, 둘의 차이가 1개이상, X가 더 많다? 불가
    // 선공 혹은 후공에서 게임이 끝났는가?? 
    
    int cntO = 0;
    int cntX = 0;
    for (int i = 0; i < board.size(); i++){
        for (int j = 0; j < board[0].length(); j++){
            if (board[i][j] == 'O') cntO++;
            else if (board[i][j] == 'X') cntX++;
        }
    }
    
    if (cntO > cntX+1 || cntO < cntX) return 0;

    bool winO = check(board, 'O');
    bool winX = check(board, 'X');
    
    if (winO && winX) return 0;
    if (winO && cntO != cntX + 1) return 0;
    if (winX && cntO != cntX) return 0;

    return 1;    
}