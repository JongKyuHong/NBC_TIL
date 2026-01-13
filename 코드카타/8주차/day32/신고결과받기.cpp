#include <string>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <iostream>

using namespace std;

vector<int> solution(vector<string> id_list, vector<string> report, int k) {
    vector<int> answer(id_list.size(), 0);
    unordered_map<string, int> idMap;
    unordered_map<string, unordered_set<string>> reportMap;
    unordered_map<string, int> scoreMap;
    unordered_map<string, int> isBannedMap;
    
    int index = 0;
    for (string& idl : id_list){
        idMap[idl] = index++;
        isBannedMap[idl] = false;
    }

    for (int i = 0; i < report.size(); i++){
        int blank = report[i].find(" ");
        
        string p1 = report[i].substr(0, blank);
        string p2 = report[i].substr(blank+1);

        reportMap[p1].insert(p2);
    }

    for (auto& rMap : reportMap) {
        string owner = rMap.first;
        for (auto& r : rMap.second){
            scoreMap[r]++;
        }
    }
    
    for (auto& sMap : scoreMap){
        if (sMap.second >= k){
            isBannedMap[sMap.first] = true;
        }
    }
    
    for (auto& rMap : reportMap) {
        string owner = rMap.first;
        int ownerIndex = idMap[owner];
        for (auto& r : rMap.second){
            if (isBannedMap[r]){
                answer[ownerIndex]++;
            }
        }
    }
    
    return answer;
}