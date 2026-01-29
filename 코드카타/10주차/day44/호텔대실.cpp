#include <string>
#include <vector>
#include <algorithm>
#include <iostream>

using namespace std;

string plusTenMinute(string& time){
    int hour = stoi(time.substr(0, 2));
    int minute = stoi(time.substr(3, 2));
            
    minute += 10;
    if (minute >= 60) {
        minute -= 60;
        hour++;
    }

    string hourStr = "";
    string minuteStr = "";

    if (hour < 10){
        hourStr = "0" + to_string(hour);
    } else {
        hourStr = to_string(hour);
    }

    if (minute < 10){
        minuteStr = "0" + to_string(minute);
    } else {
        minuteStr = to_string(minute);
    }
    
    return hourStr + ":" + minuteStr;
}

int solution(vector<vector<string>> book_time) {
    int answer = 0;
    
    vector<string> vec;

    sort(book_time.begin(), book_time.end());
        
    for (int i = 0; i < book_time.size(); i++){
        string start = book_time[i][0];
        string end = book_time[i][1];
        string newEnd = plusTenMinute(end);
        
        if (vec.empty()){
            vec.push_back(newEnd);
        } else {
            string minV = "";
            int minIndex = -1;
            
            for (int j = 0; j < vec.size(); j++){
                if (vec[j] <= start){
                    if (minV == ""){
                        minV = vec[j];
                        minIndex = j;
                    } else {
                        if (minV > vec[j]){
                            minV = vec[j];
                            minIndex = j;
                        }
                    }
                }
            }
            
            
            if (minIndex == -1){
                vec.push_back(newEnd);
            } else {
                vec[minIndex] = newEnd;    
            }
        }
    }
   
    answer = vec.size();
    return answer;
}