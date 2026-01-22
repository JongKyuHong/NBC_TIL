#include <string>
#include <vector>
#include <algorithm>
#include <iostream>

using namespace std;

bool compare(int a, int b){
    string tmpa = to_string(a);
    string tmpb = to_string(b);
    return tmpa+tmpa+tmpa+tmpa > tmpb+tmpb+tmpb+tmpb;
}

string solution(vector<int> numbers) {
    string answer = "";
    
    sort(numbers.begin(), numbers.end(), compare);
    
    for (int number : numbers){
        answer += to_string(number);
    }
    
    if (answer[0] == '0'){
        answer = "0";
    }
    
    return answer;
}