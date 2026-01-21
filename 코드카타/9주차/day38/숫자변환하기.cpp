#include <string>
#include <vector>

using namespace std;

void dfs(vector<int>& vec, int index, int n){
    if (index >= 1000001){
        return;
    }
    if (index+n < 1000001 && vec[index]+1 < vec[index+n]){
        vec[index+n] = vec[index] + 1;    
        dfs(vec, index+n, n);
    }
    if (index*2 < 1000001 && vec[index]+1 < vec[index*2]){
        vec[index*2] = vec[index] + 1;
        dfs(vec, index*2, n);
    }
    if (index*3 < 1000001 && vec[index]+1 < vec[index*3]){
        vec[index*3] = vec[index] + 1;  
        dfs(vec, index*3, n);
    }
}

int solution(int x, int y, int n) {
    int answer = 0;
    
    vector<int> vec(1000001, 1000001);
    vec[x] = 0;
    
    dfs(vec, x, n);
    
    return vec[y] == 1000001 ? -1 : vec[y];
}