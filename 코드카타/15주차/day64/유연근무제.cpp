#include <string>
#include <vector>

using namespace std;

int solution(vector<int> schedules, vector<vector<int>> timelogs, int startday) {
    int answer = 0;
    // 0index로 사용할거임
    startday--;
    
    for (int i = 0; i < schedules.size(); i++){
        int schedule = schedules[i] + 10;
        if (schedule % 100 > 59){
            int tmp = schedule % 100;
            schedule -= tmp;
            // 일의 자리 나머지
            tmp %= 10;
            schedule += 100 + tmp;
        }
        int day = startday;
        bool fail = false;
        for (int time : timelogs[i]){
            if (time <= schedule || day == 5 || day == 6){
                day++;
                day %= 7;
                continue;
            } else {
                fail = true;
                break;
            }
        }
        if (!fail){
            answer++;
        }
    }
    
    return answer;
}