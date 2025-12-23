#include <string>
#include <vector>

using namespace std;

string solution(int n) {
    string answer = "";
    string arr[] = {"수", "박"};
    for (int i = 0; i < n; i++){
        answer += arr[i%2];
    }
    return answer;
}