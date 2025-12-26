#include <string>
#include <vector>
#include <algorithm>

using namespace std;

vector<string> solution(vector<string> strings, int n) {
    vector<string> answer;
    
    sort(strings.begin(), strings.end(), [n](const auto& a, const auto& b){
        if (a[n] == b[n]){
            return a < b;
        }
        return a[n] < b[n];
    });
    
    return strings;
}