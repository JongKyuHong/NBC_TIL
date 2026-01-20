#include <string>
#include <vector>

using namespace std;

void dfs(int& answer, int index, int current, vector<int>& numbers, int target){
    if (index == numbers.size()){
        if (current == target){
            answer++;
        }
        return;
    }
    dfs(answer, index+1, current + numbers[index], numbers, target);
    dfs(answer, index+1, current - numbers[index], numbers, target);
}

int solution(vector<int> numbers, int target) {
    int answer = 0;
    
    dfs(answer, 0, 0, numbers, target);
    
    
    return answer;
}