#include <string>
#include <vector>
#include <unordered_map>

using namespace std;

vector<string> solution(vector<string> players, vector<string> callings) {
    vector<string> answer;
    unordered_map<string, int> playerIndexMap;
    
    // player를 순회해서 map으로 인덱스를 저장
    for (int i = 0; i < players.size(); i++){
        playerIndexMap[players[i]] = i;
    }
    
    // 내부 for문에서는 calling된 선수가 몇번째 인데스에 있는지 확인하고 실제로 player스왑하고 map갱신
    for (const string& calling : callings){
        int index = playerIndexMap[calling];
        playerIndexMap[calling]--;
        playerIndexMap[players[index-1]]++;
        string tmp = players[index-1];
        players[index-1] = players[index];
        players[index] = tmp;
    }
    
    return players;
}