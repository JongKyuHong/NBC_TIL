// https://school.programmers.co.kr/learn/courses/30/lessons/17683?language=cpp

#include <string>
#include <vector>
#include <unordered_map>
#include <iostream>

using namespace std;

void convert(string& m, unordered_map<string, string>& words){
    for (auto& [key, value] : words){
        auto it = m.find(key);
        while ((it = m.find(key, it)) != string::npos){
            m.replace(it, key.length(), value);
            it++;
        }
    }
}

string solution(string m, vector<string> musicinfos) {
    string answer = "";
    int maxLength = 0;
    unordered_map<string, string> words;
    
    words["A#"] = "H";
    words["C#"] = "I";
    words["D#"] = "J";
    words["F#"] = "K";
    words["G#"] = "L";
    
    string melody = "";
    convert(m, words);

    for (string& musicinfo : musicinfos){
        // ,을 기준으로 나누어야 함
        string startHour = "";
        string startMinute = "";
        string endHour = "";
        string endMinute = "";
        string musicName = "";
        string info = "";
        int flag = 0;
        int colonFlag = 0;
        
        for (char& c : musicinfo){
            if (c == ','){
                flag++;
            } else if (flag == 0){
                if (c == ':'){
                    colonFlag++;
                } else if (colonFlag == 0){
                    startHour += c;
                } else {
                    startMinute += c;
                }
            } else if (flag == 1){
                if (c == ':'){
                    colonFlag++;
                } else if (colonFlag == 1){
                    endHour += c;
                } else {
                    endMinute += c;
                }
            } else if (flag == 2){
                musicName += c;
            } else if (flag == 3){
                info += c;
            }
        }
        // cout << start << " " << end << " " << musicName << " " << info << endl;
        int hourGap = stoi(endHour) - stoi(startHour);
        int minuteGap = stoi(endMinute) - stoi(startMinute) + hourGap * 60;
        
        string target = "";
        convert(info, words);
        int infoSize = info.size();
        for (int i = 0; i < minuteGap; i++){
            target += info[i%infoSize];
        }
        // cout << m << " " << target << endl;
        // 더 재생시간이 긴거, 먼저입력된 음악제목
        if (target.find(m) != string::npos){
            if (maxLength < minuteGap){
                maxLength = minuteGap;
                answer = musicName;
            }
        }
    }
    
    
    return answer == "" ? "(None)" : answer;
}