#include <string>
#include <vector>
#include <algorithm>

using namespace std;

int solution(vector<int> citations) {
    int answer = 0;
    
    sort(citations.begin(), citations.end(), greater<int>());
    
    for (int i = 0; i < citations.size(); i++){
        int check = i+1;
        answer = max(answer, min(check, citations[i]));
    }
    
    return answer;
}