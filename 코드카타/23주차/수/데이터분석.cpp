// https://school.programmers.co.kr/learn/courses/30/lessons/250121

#include <string>
#include <vector>
#include <algorithm>
#include <unordered_map>

using namespace std;

vector<vector<int>> solution(vector<vector<int>> data, string ext, int val_ext, string sort_by) {
    vector<vector<int>> answer;
    int N = data.size();
    
    unordered_map<string, int> um = {{"code", 0}, {"date", 1}, {"maximum", 2}, {"remain", 3}};
    for (size_t i = 0; i < N; ++i){
        int index = um[ext];
        if (data[i][index] < val_ext) answer.push_back(data[i]);
    }
    
    sort(answer.begin(), answer.end(), [&um, sort_by](const auto& a, const auto& b){
        int index = um[sort_by];
        return a[index] < b[index];
    });
    
    
    return answer;
}