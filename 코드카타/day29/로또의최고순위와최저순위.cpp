#include <string>
#include <vector>
#include <unordered_map>

using namespace std;

vector<int> solution(vector<int> lottos, vector<int> win_nums) {
    vector<int> answer;
    unordered_map<int, int> score = {{6, 1}, {5, 2}, {4, 3}, {3, 4}, {2, 5}, {1, 6}, {0, 6}};
    int ans = 0;
    int zeroCnt = 0;
    for (int lotto : lottos){
        if (lotto == 0){
            zeroCnt++;
            continue;
        }
        for (int win_num : win_nums){
            if (lotto == win_num){
                ans++;
            }
        }
    }

    answer.push_back(score[ans+zeroCnt]);
    answer.push_back(score[ans]);
    
    return answer;
}