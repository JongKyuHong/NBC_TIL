#include <string>
#include <vector>
#include <iostream>
#include <climits>
#include <algorithm>

using namespace std;

vector<string> solution(vector<vector<int>> line) {
    vector<pair<long long, long long>> vec;
    long long minX = LLONG_MAX;
    long long maxX = LLONG_MIN;
    long long minY = LLONG_MAX;
    long long maxY = LLONG_MIN;
    
    for (int i = 0; i < line.size(); i++){
        long long baseA = line[i][0];
        long long baseB = line[i][1];
        long long baseC = line[i][2];
        for (int j = i+1; j < line.size(); j++){
            long long targetA = line[j][0];
            long long targetB = line[j][1];
            long long targetC = line[j][2];
            
            long long tmp = baseA*targetB - targetA*baseB;
            if (tmp == 0) continue;
            long long valueX = baseB*targetC - targetB*baseC;
            long long valueY = targetA*baseC - baseA*targetC;
            long long x, y;
            // 정수쌍이 있으면
            if (valueX % tmp == 0 && valueY % tmp == 0){
                x = valueX/tmp;
                y = valueY/tmp;
                minX = min(x, minX);
                minY = min(y, minY);
                maxX = max(x, maxX);
                maxY = max(y, maxY);
                vec.push_back({x, y});
            }
        }
    }
    
    string row(maxX - minX + 1, '.');
    vector<string> answer(maxY - minY + 1, row);
    
    for (auto v : vec){
        int r = maxY - v.second;
        int c = v.first - minX;
        answer[r][c] = '*';
    }
    return answer;
}