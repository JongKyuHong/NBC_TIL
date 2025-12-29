#include <string>
#include <vector>
#include <iostream>

using namespace std;

string solution(int a, int b) {
    string result = "";
    string answer[] = {"THU", "FRI","SAT","SUN","MON","TUE","WED"};
    int cal[] = {31,29,31,30,31,30,31,31,30,31,30,31};
    
    int total = 0;
    for (int i = 0; i < a-1; i++){
        total += cal[i];
    }
    total+=b;
    
    return answer[total%7];
}