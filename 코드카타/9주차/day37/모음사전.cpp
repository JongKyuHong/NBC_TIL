#include <string>
#include <vector>
#include <iostream>
#include <algorithm>

using namespace std;
int index = 0;
int current = -1;

void dfs(string value, vector<char>& vec, string& target){
    current++;
    if (value.length() == target.length()){
        if (target == value){
            index = current;
        }
    }
    
    if (value.length() == 5){
        return;
    }
    for (int i = 0; i < vec.size(); i++){
        dfs(value+vec[i], vec, target);
    }
}

int solution(string word) {
    vector<char> vec = {'A', 'E', 'I', 'O', 'U'};
    
    dfs("", vec, word);
    
    return index;
}