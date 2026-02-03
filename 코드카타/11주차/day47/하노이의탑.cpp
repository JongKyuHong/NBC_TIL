#include <string>
#include <vector>

using namespace std;

void move(int start, int end, int mid, int n, vector<vector<int>>& answer){
    if (n == 1){
        answer.push_back({start, end});
        return;
    }
    move(start, mid, end, n-1, answer);
    answer.push_back({start, end});
    move(mid, end, start, n-1, answer);
}

vector<vector<int>> solution(int n) {
    vector<vector<int>> answer;
    move(1, 3, 2, n, answer);
    return answer;
}