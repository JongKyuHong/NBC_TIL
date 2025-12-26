#include <string>
#include <vector>
#include <algorithm>

using namespace std;

vector<int> solution(vector<int> array, vector<vector<int>> commands) {
    vector<int> answer;
    
    for (auto command : commands){
        vector<int> newOne(array.begin()+command[0]-1, array.begin()+command[1]);
        sort(newOne.begin(), newOne.end());
        answer.push_back(newOne[command[2]-1]);
    }
    
    
    return answer;
}