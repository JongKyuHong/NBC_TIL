// https://school.programmers.co.kr/learn/courses/30/lessons/42579?language=cpp

#include <string>
#include <vector>
#include <unordered_map>
#include <algorithm>
#include <iostream>

using namespace std;

vector<int> solution(vector<string> genres, vector<int> plays) {
    vector<int> answer;

    unordered_map<string, long long> totalGenresPlayTime;
    unordered_map<string, vector<pair<int, long long>>> genresPlayTime;
    
    int N = genres.size();
    
    for (int i = 0; i < N; ++i){
        totalGenresPlayTime[genres[i]] += plays[i];
        genresPlayTime[genres[i]].push_back({i, plays[i]});
    }
    
    vector<pair<string, long long>> vec(totalGenresPlayTime.begin(), totalGenresPlayTime.end());
    sort(vec.begin(), vec.end(), [](const auto& a, const auto& b){
       if (a.second != b.second) return a.second > b.second; 
    });
    
    for (const auto& v : vec){
        sort(genresPlayTime[v.first].begin(), genresPlayTime[v.first].end(), [](const auto& a, const auto& b){
            if (a.second != b.second) return a.second > b.second;
            return a.first < b.first;
        });
        int flag = 0;
        for (const auto& playTime : genresPlayTime[v.first]){
            answer.push_back(playTime.first);
            flag++;
            if (flag == 2) break;
        }
    }
    
    return answer;
}