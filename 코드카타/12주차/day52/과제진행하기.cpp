#include <string>
#include <vector>
#include <stack>
#include <algorithm>
#include <iostream>
#include <climits>

using namespace std;

struct Task{
    string name;
    int start;
    int duration;
};

int convertMinute(string& time){
     int hour = stoi(time.substr(0, 2));
    int minute = stoi(time.substr(3, 2));
    return hour * 60 + minute;
}

vector<string> solution(vector<vector<string>> plans) {
    vector<string> answer;
    
    vector<Task> vec;
    
    for (auto& plan : plans){
        vec.push_back({plan[0], convertMinute(plan[1]), stoi(plan[2])});
    }
    
    sort(vec.begin(), vec.end(), [](auto& a, auto& b){
        return a.start < b.start;
    });
    
    vector<pair<string, int>> st;
    
    for (int i = 0; i < vec.size(); i++){
        int currentStart = vec[i].start;
        int currentDuration = vec[i].duration;
        
        int nextStart = (i+1 < vec.size()) ? vec[i+1].start : INT_MAX;
        int gap = nextStart - currentStart;
        
        if (currentDuration <= gap){
            answer.push_back(vec[i].name);
            gap -= currentDuration;
            
            while (gap > 0 && !st.empty()){
                auto [name, time] = st.back();
                st.pop_back();
                
                if (time <= gap){
                    answer.push_back(name);
                    gap -= time;
                } else {
                    time -= gap;
                    gap = 0;
                    st.push_back({name, time});
                }
            }
        } else {
            st.push_back({vec[i].name, currentDuration - gap});
        }
    }
    
    while (!st.empty()){
        answer.push_back(st.back().first);
        st.pop_back();
    }
    
    return answer;
}