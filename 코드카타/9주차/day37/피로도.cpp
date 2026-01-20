#include <string>
#include <vector>

using namespace std;
int answer = 0;

void dfs(vector<int>& visited, int count, int remainK, vector<vector<int>>& dungeons){
    answer = max(answer, count);
    
    for (int i = 0; i < visited.size(); i++){
        int require = dungeons[i][0];
        int exhaustion = dungeons[i][1];
        if (visited[i] == 0 && remainK >= require){
            visited[i] = 1;
            dfs(visited, count+1, remainK-exhaustion, dungeons);
            visited[i] = 0;
        }    
        
    }
}


int solution(int k, vector<vector<int>> dungeons) {
    vector<int> visited(dungeons.size(), 0);
    dfs(visited, 0, k, dungeons);
    
    return answer;
}