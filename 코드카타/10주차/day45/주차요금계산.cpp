#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <cmath>
#include <sstream>

using namespace std;

vector<int> solution(vector<int> fees, vector<string> records) {
    vector<int> answer;
    
    int normal_time = fees[0];
    int normal_fee = fees[1];
    int unit_time = fees[2];
    int unit_fee = fees[3];

    map<string, int> in_time; 
    map<string, int> total_time;
    
    for (const string& record : records) {
        stringstream ss(record);
        string time, carNumber, type;
        ss >> time >> carNumber >> type;

        int hour = stoi(time.substr(0, 2));
        int minute = stoi(time.substr(3, 2));
        int t = hour * 60 + minute;

        if (type == "IN") {
            in_time[carNumber] = t;
        } else {
            total_time[carNumber] += (t - in_time[carNumber]);
            in_time.erase(carNumber);
        }
    }

    int last_time = 23 * 60 + 59;
    for (auto const& [carNumber, t] : in_time) {
        total_time[carNumber] += (last_time - t);
    }
    for (auto const& [carNumber, time] : total_time) {
        if (time <= normal_time) {
            answer.push_back(normal_fee);
        } else {
            int extra_time = time - normal_time;
            int units = (extra_time + unit_time - 1) / unit_time;
            int total_fee = normal_fee + units * unit_fee;
            
            answer.push_back(total_fee);
        }
    }

    return answer;
}