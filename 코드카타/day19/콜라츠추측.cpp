#include <string>
#include <vector>

using namespace std;

int solution(int num) {
    int answer = 0;
    long long tmp = num;
    if (num == 1){
        return 0;
    }
    while (true){
        if (tmp % 2 == 0){
            tmp /= 2;
        } else {
            tmp = (tmp * 3) + 1;
        }
        answer++;
        if (answer == 500){
            return -1;
        }
        if (tmp == 1){
            break;
        }
    }
    return answer;
}