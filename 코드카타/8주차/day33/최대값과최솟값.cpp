#include <string>
#include <vector>
#include <sstream>
#include <iostream>

using namespace std;

string solution(string s) {
    string answer = "";
    int minV = 1098765432;
    int maxV = -1098765432;
    istringstream ss(s);
    string token;
    
    while (ss >> token){
        int sInt = stoi(token);
        minV = min(minV, sInt);
        maxV = max(maxV, sInt);
    }
    
    answer += to_string(minV);
    answer += " ";
    answer += to_string(maxV);
    
    return answer;
}