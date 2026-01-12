#include <string>
#include <vector>
#include <unordered_map>
using namespace std;

string solution(vector<string> survey, vector<int> choices) {
    string answer = "";
    unordered_map<char, int> mm;
    
    mm['R'] = 0;
    mm['T'] = 0;
    mm['C'] = 0;
    mm['F'] = 0;
    mm['J'] = 0;
    mm['M'] = 0;
    mm['A'] = 0;
    mm['N'] = 0;
    
    for (int i = 0; i < survey.size(); i++){
        if (choices[i] > 4){
            mm[survey[i][1]] += choices[i] - 4;
        } else if (choices[i] < 4){
            mm[survey[i][0]] += 4 - choices[i];
        }
    }
    
    if (mm['T'] > mm['R']){
        answer += 'T';
    } else {
        answer += 'R';
    }
    
    if (mm['C'] >= mm['F']){
        answer += 'C';
    } else {
        answer += 'F';
    }
    
    if (mm['J'] >= mm['M']){
        answer += 'J';
    } else {
        answer += 'M';
    }
    
    if (mm['A'] >= mm['N']){
        answer += 'A';
    } else {
        answer += 'N';
    }
    
    return answer;
}