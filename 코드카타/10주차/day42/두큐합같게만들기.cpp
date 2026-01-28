#include <string>
#include <vector>
#include <deque>
#include <iostream>

using namespace std;


int solution(vector<int> queue1, vector<int> queue2) {
    int answer = 0;
    deque<int> q1;
    deque<int> q2;
    long long sum1 = 0;
    long long sum2 = 0;
    for (int i = 0; i < queue1.size(); i++){
        sum1+=queue1[i];
        q1.push_back(queue1[i]);
    }
    for (int i = 0; i < queue2.size(); i++){
        sum2+=queue2[i];
        q2.push_back(queue2[i]);
    }
    
    int countMax = (queue1.size() + queue2.size())*2;
    
    while (sum1 != sum2 && answer < countMax){
        // cout << "sum1 : " << sum1 << endl;
        // cout << "sum2 : " << sum2 << endl;
        // cout << "answer : " << answer << endl;
        if (sum1 > sum2){
            int value = q1.front();
            q1.pop_front();
            q2.push_back(value);
            sum1 -= value;
            sum2 += value;
        } else {
            int value = q2.front();
            q2.pop_front();
            q1.push_back(value);
            sum1 += value;
            sum2 -= value;
        }
        answer++;
    }
    

    return answer == countMax ? -1 : answer;
}