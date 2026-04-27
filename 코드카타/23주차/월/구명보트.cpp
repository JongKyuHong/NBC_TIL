// https://school.programmers.co.kr/learn/courses/30/lessons/42885?language=cpp

#include <string>
#include <vector>
#include <algorithm>

using namespace std;

int solution(vector<int> people, int limit) {
    int answer = 0;

    sort(people.begin(), people.end(), greater<int>());
    
    int start = 0;
    int end = people.size()-1;
    
    while (start <= end){
        int current = people[start++];
        if (limit - current >= people[end]){
            end--;
        }
        answer++;
    }
    
    return answer;
}