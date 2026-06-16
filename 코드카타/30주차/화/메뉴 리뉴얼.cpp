// https://school.programmers.co.kr/learn/courses/30/lessons/72411?language=cpp

#include <string>
#include <vector>
#include <unordered_map>
#include <algorithm>
#include <iostream>

using namespace std;

vector<string> solution(vector<string> orders, vector<int> course) {
    vector<string> answer;
    
    
    sort(orders.begin(), orders.end(), [](auto& a, auto& b){
       if (a.size() != b.size()){
           return a.size() < b.size();
       }
    });
    for (int cour : course){
        unordered_map<string, int> um;
        for (string& order : orders){
            sort(order.begin(), order.end());
            int n = order.size();
            if (cour > n) continue;
            vector<int> mask(n, 0);    
            for (int i = n-cour; i < n; i++){
                mask[i] = 1;
            }

            do{
                string tmp = "";
                for (int i = 0; i < n; i++){
                    if (mask[i] == 1) tmp += order[i];
                }    
                sort(tmp.begin(), tmp.end());
                um[tmp]++;
            } while (next_permutation(mask.begin(), mask.end()));
        }
        int maxValue = 0;
        vector<string> vec;
        for (const auto& [key, value] : um){
            if (value > 1){
                if (value > maxValue){
                    maxValue = value;
                    vec.clear();
                    vec.push_back(key);
                } else if (value == maxValue){
                    vec.push_back(key);
                }
            }
        }
        for (int i = 0; i < vec.size(); i++){
            answer.push_back(vec[i]);
        }
    }
    
    sort(answer.begin(), answer.end());
    return answer;
}