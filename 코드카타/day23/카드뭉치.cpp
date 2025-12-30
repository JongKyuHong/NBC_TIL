#include <string>
#include <vector>

using namespace std;

bool compare(int index, int index1, int index2, const vector<string>& cards1, const vector<string>& cards2, vector<string> ss, const vector<string>& goal){
    for (int i = 0; i < ss.size(); i++){
        if (ss[i] != goal[i]){
            return false;
        }
    }
    
    if (index == goal.size()) {
        return true;
    }
    
    if (index1 < cards1.size() && cards1[index1] == goal[index]){
        ss.push_back(cards1[index1]);
        if (compare(index+1, index1+1, index2, cards1, cards2, ss, goal)){
            return true;
        }
        ss.pop_back();
    } 
    if (index2 < cards2.size() && cards2[index2] == goal[index]){
        ss.push_back(cards2[index2]);
        if (compare(index+1, index1, index2+1, cards1, cards2, ss, goal)){
            return true;
        }
        ss.pop_back();
    }
    
    return false;
}


string solution(vector<string> cards1, vector<string> cards2, vector<string> goal) {
    int index = 0;
    int index1 = 0;
    int index2 = 0;
    vector<string> ss;
    
    if (compare(index, index1, index2, cards1, cards2, ss, goal)){
        return "Yes";
    } else {
        return "No";
    }
}