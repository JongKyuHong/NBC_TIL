#include <string>
#include <vector>

using namespace std;

int gcd(int a, int b){
    while (b!=0){
        int tmp = b;
        b = a % b;
        a = tmp;
    }
    return a;
}

long long lcd(int a, int b){
    return (long long)a*b / gcd(a, b);
}

vector<int> solution(int n, int m) {
    vector<int> answer;
    answer.push_back(gcd(n, m));
    answer.push_back(lcd(n, m));
    return answer;
}