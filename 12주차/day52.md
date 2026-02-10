# 코드카타

## NQueen

- [문제](https://school.programmers.co.kr/learn/courses/30/lessons/12952)

```c++
#include <string>
#include <vector>
#include <iostream>

using namespace std;

bool isSafe(int row, int col, vector<vector<int>>& visited, int n){
    for (int j = 0; j < n; j++){
        if (row-j >= 0){
            if (col - j >= 0){
                if (visited[row-j][col-j]){
                    return false;
                }
            }
            if (visited[row-j][col]){
                return false;
            }
            if (col + j < n){
                if (visited[row-j][col+j]){
                    return false;
                }
            }
        }
    }
    return true;
}

int answer;
void dfs(int row, vector<vector<int>>& visited, int n){
    if (row == n-1){
        answer++;
        return;
    }

    for (int i = 0; i < n; i++){
        if (isSafe(row+1, i, visited, n)){
            visited[row+1][i] = 1;
            dfs(row+1, visited, n);
            visited[row+1][i] = 0;
        }
    }
}

int solution(int n) {
    answer = 0;

    vector<vector<int>> visited(n, vector<int>(n, 0));
    for (int i = 0; i < n; i++){
        visited[0][i] = 1;
        dfs(0, visited, n);
        visited[0][i] = 0;
    }


    return answer;
}
```

- 백트래킹의 대표격인문제
- 내가 알기로는 while문을 써서 더 간단하게 푸는방법이 있는데 기억이 안나서 그냥 for문을 여러번 돌려서 풀었다.

## 과제 진행하기

- [문제](https://school.programmers.co.kr/learn/courses/30/lessons/176962)

```c++
#include <string>
#include <vector>
#include <stack>
#include <algorithm>
#include <iostream>
#include <climits>

using namespace std;

struct Task{
    string name;
    int start;
    int duration;
};

int convertMinute(string& time){
     int hour = stoi(time.substr(0, 2));
    int minute = stoi(time.substr(3, 2));
    return hour * 60 + minute;
}

vector<string> solution(vector<vector<string>> plans) {
    vector<string> answer;

    vector<Task> vec;

    for (auto& plan : plans){
        vec.push_back({plan[0], convertMinute(plan[1]), stoi(plan[2])});
    }

    sort(vec.begin(), vec.end(), [](auto& a, auto& b){
        return a.start < b.start;
    });

    vector<pair<string, int>> st;

    for (int i = 0; i < vec.size(); i++){
        int currentStart = vec[i].start;
        int currentDuration = vec[i].duration;

        int nextStart = (i+1 < vec.size()) ? vec[i+1].start : INT_MAX;
        int gap = nextStart - currentStart;

        if (currentDuration <= gap){
            answer.push_back(vec[i].name);
            gap -= currentDuration;

            while (gap > 0 && !st.empty()){
                auto [name, time] = st.back();
                st.pop_back();

                if (time <= gap){
                    answer.push_back(name);
                    gap -= time;
                } else {
                    time -= gap;
                    gap = 0;
                    st.push_back({name, time});
                }
            }
        } else {
            st.push_back({vec[i].name, currentDuration - gap});
        }
    }

    while (!st.empty()){
        answer.push_back(st.back().first);
        st.pop_back();
    }

    return answer;
}
```

- 단순한 빡구현 문제
- 중간에 풀다가 남은 시간사이에도 스택에서 꺼내서 풀어야하는데 그 부분을 안넣어서 너무 귀찮아서 풀이들을 찾아다니면서 최대한 귀찮지 않은 방법으로 푼 풀이로 풀게되었다.
- struct를 vector에다가 쓰는부분, 그리고 분으로 통일해서 계산하는 부분이 인상깊은 풀이였다.
