#include <string>
#include <vector>

using namespace std;

int solution(int n, int m, vector<int> section) {
    int answer = 0;
    int end = 0;
    for (int sec : section){
        if (sec <= end){
            continue;
        }
        answer++;
        end = sec + m - 1;
    }
    return answer;
}