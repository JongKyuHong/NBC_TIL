#include <string>
#include <vector>
#include <iostream>
#include <map>

using namespace std;

vector<double> solution(int k, vector<vector<int>> ranges) {
    vector<double> answer;

    // 1 -2  
    // k가 몇번만에 1이되느냐
    vector<int> path;
    path.push_back(k);
    int n = 0;
    while (k > 1){
        if (k % 2){
            k *= 3;
            k++;
        } else {
            k /= 2;
        }
        path.push_back(k);
        n++;
    }

    // 우박수열을 보기전에 미리 만개의 크기를 구해놓는게 낫지않나?
    // um[0]은 0과 1사이의 크기
    map<pair<int, int>, double> um;
    
    // 크기는 어떻게 구함?
    for (int i = 0; i < path.size()-1; i++){
        double maxH = max(path[i], path[i+1]);
        double minH = min(path[i], path[i+1]);
        
        double value = maxH;
        value -= ((maxH - minH) / 2);
        um[{i, i+1}] = value;
    }
    
    for (int i = 0; i < ranges.size(); i++){
        int a = ranges[i][0];
        // 애초에 마이너스로 들어가있음
        int b = ranges[i][1];
        double value = 0;
        if (a == 0 && b == 0){
            for (int j = 0; j < path.size()-1; j++){
                value += um[{j, j+1}];
            }
        } else if (a > n+b){
            value = -1.0;
        } else {
            for (int j = a; j < n+b; j++){
                value += um[{j, j+1}];
            }    
        }

        answer.push_back(value);
    }
    
    return answer;
}