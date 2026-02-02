#include <string>
#include <vector>
#include <algorithm>
#include <iostream>

using namespace std;

int solution(vector<vector<int>> data, int col, int row_begin, int row_end) {
    int answer = -1;
    
    sort(data.begin(), data.end(), [col](auto& a, auto& b){
        if (a[col-1] != b[col-1]) return a[col-1] < b[col-1];
        return a[0] > b[0];
    });
    
    for (int i = row_begin-1; i <= row_end-1; i++){
        vector<int> target = data[i];
        int sumV = 0;
        for (int t : target){
            sumV += t % (i+1);
        }
        
        if (answer == -1){
            answer = sumV;
        } else {
            answer ^= sumV;
        }
        
    }

    
    
    return answer;
}