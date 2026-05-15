// https://school.programmers.co.kr/learn/courses/30/lessons/468370

#include <string>
#include <vector>
#include <unordered_map>
#include <unordered_set>

using namespace std;

int solution(string message, vector<vector<int>> spoiler_ranges) {
    int answer = 0;
    
    // 왼쪽 -> 오른쪽 순서로 스포 방지 구간을 하나씩 클릭해 공개되는 단어들 중, 중요한 단어가 몇 개 인지 확인하려 함
    // 단어는 숫자 + 소문자, 중요한 단어가 몇 개인지 확인해야 한다.
    // 문자들의 인덱스 중 하나 이상이 스포방지 구간에 포함될 경우 해당 단어는 스포일러 방지 단어로 간주
    // 한 단어가 여러 스포일러 방지 구간에 있을수도 한 스포일러방지 구간에 여러 단어가 있을 수도 있다.
    // 중요한 단어는 스포 방지 단어, 스포방지구간이 아닌곳에 있으면 안됨, 이전에 공개된 스포 방지 단어와 중복되지 않아야 함, 여러 단어가 동시에 공개된 경우, 왼쪽부터 순서대로 하나씩 중요한 단어인지 판단함

    // 2풀이
    // 1. spoiler_ranges를 먼저 순회해서 visited처럼 1로 처리를 해놓음
    // 2. message를 순회하면서 visited가 1이면 스포일러 처리된 단어라고 판단하고 따로 저장
    // 3. visited가 순수하게 0인 단어도 따로 저장
    // 4. 나중에 처리
    unordered_set<string> spoWords;
    unordered_map<string, int> noneSpoWords;
    
    
    int N = message.size();
    vector<bool> visited(N, false);
    for (const auto& spoiler_range : spoiler_ranges){
        int start = spoiler_range[0];
        int end = spoiler_range[1];
        for (int i = start; i <= end; ++i){
            visited[i] = true;
        }
    }
    
    int index = 0;
    while (index < N){
        string tmp = "";
        int startIndex = index;
        bool isSpoiler = false;
        while (index < N){
            if (message[index] != ' ') {
                if (visited[index]) isSpoiler = true;
                tmp += message[index++];
            } else {
                // 공백 넘기기
                index++;
                break;
            };
        }
        
        if (isSpoiler) spoWords.insert(tmp);
        else noneSpoWords[tmp] = 1;
    }
    
    vector<string> vec(spoWords.begin(), spoWords.end());
    
    for (auto& v : vec) if (noneSpoWords.find(v) == noneSpoWords.end()) answer++;
    
    
    return answer;
}