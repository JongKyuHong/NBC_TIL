#include <string>
#include <vector>
#include <unordered_map>

using namespace std;

vector<int> solution(string s) {
    vector<int> answer;
    unordered_map<char, int> um;
    
    for (int i = 0; i < s.length(); i++){
        if (um.find(s[i]) == um.end()){
            answer.push_back(-1);
            um[s[i]] = i;
        } else {
            answer.push_back(i - um[s[i]]);
            um[s[i]] = i;
        }
    }
    
    return answer;
}