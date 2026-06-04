// https://school.programmers.co.kr/learn/courses/30/lessons/42888?language=cpp

#include <string>
#include <vector>
#include <unordered_map>
#include <iostream>

using namespace std;

vector<string> solution(vector<string> record) {
    vector<string> answer;
    
    // 들어왔다, 나갔다 라는 메시지 밖에없음
    // id별로 닉네임이 있는데 최종적으로 어떤 아이디에 어떤 닉네임이 매칭되어있는지 확인하면 된다.
    unordered_map<string, string> matchMap;
    
    // 메시지를 관리하는 vector를 만들어서 순서를 지키며 저장해놓기
    vector<pair<string, string>> message;
    
    for (const string& rec : record){
        string command = "";
        string uid = "";
        string nickname = "";
        int flag = 0;
        for (const char& r : rec){
            if (r == ' '){
                flag++;
            } else if (flag == 0){
                command += r;
            } else if (flag == 1){
                uid += r;
            } else if (flag == 2){
                nickname += r;
            }
        }
        // cout << "command : " << command << " uid : " << uid << " nickname : " << nickname << endl;
        if (command == "Enter" || command == "Leave"){
            message.push_back({uid, command});
        }
        if (command == "Enter" || command == "Change"){
            // 나갔다가 새로운 닉네임으로 들어온것도 갱신
            matchMap[uid] = nickname;
        }
    }
    
    for (int i = 0; i < message.size(); i++){
        string uid = message[i].first;
        string command = message[i].second;
        
        string result = "";
        result += matchMap[uid];
        result += "님이 ";
        result += command == "Enter" ? "들어왔습니다." : "나갔습니다.";
        answer.push_back(result);
    }
    
    return answer;
}