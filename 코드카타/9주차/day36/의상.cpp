#include <string>
#include <vector>
#include <unordered_map>
#include <vector>

using namespace std;

void dfs(int index, vector<int>& counts, int& answer){
    if (index == counts.size()){
        answer++;
        return;
    }
    
    for (int _ = 0; _ < counts[index]; _++){
        dfs(index+1, counts, answer);
    }
    
    dfs(index+1, counts, answer);
}

int solution(vector<vector<string>> clothes) {
    int answer = 0;
    unordered_map<string, int> typeCounts;
    vector<int> counts;
    
    for (auto& cloth : clothes){
        typeCounts[cloth[1]]++;
    }
    
    for (auto& tc : typeCounts){
        counts.push_back(tc.second);
    }
    
    dfs(0, counts, answer);
    
    return answer-1;
}