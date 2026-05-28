// https://school.programmers.co.kr/learn/courses/30/lessons/49994?language=cpp

#include <string>
#include <map>

using namespace std;

int solution(string dirs) {
    int answer = 0;
    map<pair<pair<int, int> , pair<int, int>>, int> um;

    int r = 0;
    int c = 0;
    
    for (char dir : dirs){
        int nr = r;
        int nc = c;
        
        if (dir == 'U') nr += 1;
        else if (dir == 'D') nr -= 1;
        else if (dir == 'R') nc +=1;
        else if (dir == 'L') nc -= 1;
        
        if (nr > 5 || nr < -5 || nc > 5 || nc < -5) continue;
        
        pair<int, int> start = {r, c};
        pair<int, int> end = {nr, nc};
        um[{min(start, end), max(start, end)}] += 1;
        r = nr;
        c = nc;
    }
    
    for (auto [key, value] : um){
        answer++;
    }
    
    
    return answer;
}