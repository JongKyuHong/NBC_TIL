# 코드카타

## 큰수 만들기

```c++
#include <string>
#include <vector>
#include <stack>

using namespace std;

string solution(string number, int k) {
    string answer = "";

    stack<char> st;
    int index = 0;
    while (index < number.length()){
        while (!st.empty() && st.top() < number[index]){
            st.pop();
            k--;
            if (k == 0){
                break;
            }
        }
        if (k == 0){
            break;
        }

        st.push(number[index]);
        index++;
    }

    // 테스트케이스가 "10", 1이 들어왔을때 "10"이 출력됨 k가 남았을때 처리를 안함
    // 스택 위쪽이 작은값이므로 k개수만큼 빼줌
    while (k > 0 && !st.empty()) {
        st.pop();
        k--;
    }

    while(!st.empty()){
        answer = st.top() + answer;
        st.pop();
    }

    for (int i = index; i < number.length(); i++){
        answer += number[i];
    }

    return answer;
}
```

- [링크](https://school.programmers.co.kr/learn/courses/30/lessons/42883)
- 문자열에서 k개의 문자를 제거해서 가장 큰 숫자문자열을 만드는 문제
- 스택을 이용해서 쉽게풀었는데 테스트케이스 12번만 틀렸다.
  - 만약에 문자열이 이미 내림차순이 되어있는경우에 k개를 빼주지않았어서 while문을 하나 더 추가해서 k가 0이될때까지 스택에서 제거해주는 로직을 넣고 통과되었다.

## 점 찍기

```c++
#include <string>
#include <vector>
#include <cmath>
#include <iostream>

using namespace std;

long long solution(int k, int d) {
    long long answer = 0;

    for (int i = 0; i <= d; i += k){
        long long tmp = sqrt(pow(d,2) - pow(i ,2));
        answer += tmp / k;
        answer++;
    }

    return answer;
}
```

- [링크](https://school.programmers.co.kr/learn/courses/30/lessons/140107)
- d의 길이까지 몇개의 좌표가 있는가
- 행이나 열을 하나 고정하고 그것을 줄여가며 몇개의 점이 있는지 확인
- 매 순회마다 answer를 더해서 0인경우도 세주어야함
