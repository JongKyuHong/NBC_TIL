#include <string>
#include <vector>
#include <algorithm>
#include <iostream>

using namespace std;

vector<int> solution(vector<string> wallpaper) {
    vector<int> answer;
    
    // 행의 최소값, 열의 최소값부터 
    // 행의 최대값, 열의 최대값까지
    
    int minCol = wallpaper[0].length();
    int minRow = wallpaper.size();
    int maxRow = 0;
    int maxCol = 0;
    
    for (int i = 0; i < wallpaper.size(); i++){
        for (int j = 0; j < wallpaper[0].length(); j++){
            if (wallpaper[i][j] == '#'){
                minCol = min(minCol, j);
                minRow = min(minRow, i);
                maxRow = max(maxRow, i);
                maxCol = max(maxCol, j);
            }
        }
    }
    
    return {minRow, minCol, maxRow+1, maxCol+1};
}