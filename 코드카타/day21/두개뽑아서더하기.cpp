#include <string>
#include <vector>
#include <unordered_set>
#include <algorithm>

using namespace std;

vector<int> solution(vector<int> numbers) {
    unordered_set<int> us;
    for (int i = 0; i < numbers.size()-1; i++){
        for (int j = i+1; j < numbers.size(); j++){
            us.insert(numbers[i]+numbers[j]);
        }
    }
    
    vector<int> answer(us.begin(), us.end());
    
    sort(answer.begin(), answer.end());
    
    return answer;
}