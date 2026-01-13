# 코드카타

## 개인정보수집유효기간문제

```c++
#include <string>
#include <vector>
#include <unordered_map>

using namespace std;

int calculateDate(string& date){
    int year = stoi(date.substr(0, 4));
    int month = stoi(date.substr(5, 2));
    int day = stoi(date.substr(8, 2));

    int answer = year*12 + month-1;
    answer *= 28;
    answer += day;
    return answer;
}

vector<int> solution(string today, vector<string> terms, vector<string> privacies) {
    vector<int> answer;
    unordered_map<char, int> monthTerm;

    for (string& term : terms){
        char t = term[0];
        int month = stoi(term.substr(2));
        monthTerm[t] = month;
    }

    int calToday = calculateDate(today);

    for (int i = 0; i < privacies.size(); i++){
        string date = privacies[i].substr(0, 10);
        char termType = privacies[i][11];

        int start = calculateDate(date);
        int end = start + monthTerm[termType] * 28;

        if (end <= calToday){
            answer.push_back(i+1);
        }
    }
    return answer;
}
```

- substr활용(시작위치, 길이)
- 모든 월이 28일로 고정되어서 쉽게 풀었다.
