#include <string>
#include <vector>

using namespace std;

int rotate(vector<vector<int>>& vec, vector<int>& query){
    int r1 = query[0]-1;
    int c1 = query[1]-1;
    int r2 = query[2]-1;
    int c2 = query[3]-1;
    
    int tmp = vec[r1][c1];
    int minV = tmp;
    
    for (int i = r1; i < r2; i++){
        vec[i][c1] = vec[i+1][c1];
        minV = min(minV, vec[i][c1]);
    }
    
    for (int i = c1; i < c2; i++){
        vec[r2][i] = vec[r2][i+1];
        minV = min(minV, vec[r2][i]);
    }
    
    for (int i = r2; i > r1; i--){
        vec[i][c2] = vec[i-1][c2];
        minV = min(minV, vec[i][c2]);
    }
    
    for (int i = c2; i > c1 + 1; i--){
        vec[r1][i] = vec[r1][i-1];
        minV = min(minV, vec[r1][i]);
    }
    
    vec[r1][c1+1] = tmp;
    return minV;
}

vector<int> solution(int rows, int columns, vector<vector<int>> queries) {
    vector<int> answer;
    vector<vector<int>> vec(rows, vector<int>(columns, 0));
    int nums = 1;
    for (int i = 0; i < rows; i++){
        for (int j = 0; j < columns; j++){
            vec[i][j] = nums++;
        }
    }
    
    
    for (auto query : queries){
        int minV = rotate(vec, query);
        answer.push_back(minV);
    }
    
    return answer;
}