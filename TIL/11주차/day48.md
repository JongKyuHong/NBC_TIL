# 커리어 데이

## AI

- 강의를 듣고 이해하고, 과제를 만들었으니까 이제 AI를 사용해도된다. 하지만 절대로 복사/붙여넣기 하지말고 꼭 설명을 읽자 꼭 설명을 읽고 TIL에 적자
- 질문할때 주의사항도 같이 적어달라하면 더 좋을것 같다.

## 오류기록

- 시행착오 끝에 실력이 늘기때문에 시행착오를 많이 겪어보자 그리고 꼭기록하자
  - 기록할때는 어떤 방식을 시도했고 어떤 결과를 기대하는지를 적어보자
- 다 공부하고 진행하는게 아니고, 하면서 공부하는 느낌으로 하자

## TIL

- TIL은 정말 중요하다.
- 면접관 입장에서 얼마나 깊이있게 다뤘는지 확인
- 면접에서 TIL에서 적었던 서브젝트에 대해서 물어볼 수 있으니 나도 다시 리마인드 해야한다.
- TIL을 매주 정리하자

## 알고리즘 공부법

- 단순 암기보다는
- 왜?? 인지 궁금해하자
- 모든것은 스토리가 있다. -> 스토리 기반으로 기억해보자
- 걔네끼리 비교 (이상형 월드컵마냥 장단점)
- 하다가 너무 안풀리면 솔루션을 보고 -> 머리로 이거를 카피하자 반복하다보면 이해가 될때가 있다.

# 코드카타

## 마법의 엘리베이터

```c++
#include <string>
#include <vector>
#include <string>
#include <stack>
#include <iostream>

using namespace std;

int solution(int storey) {
    int answer = 0;

    string target = to_string(storey);
    stack<int> st;

    for (char t : target){
        st.push(t - '0');
    }

    while (!st.empty()){
        int s = st.top();
        st.pop();
        if (s > 5 && s < 10){
            answer += 10 - s;
            if (!st.empty()){
                st.top() = st.top() + 1;
            } else {
                answer += 1;
            }
        } else if (s < 5){
            answer += s;
        } else if (s == 5){
            if (!st.empty()){
                if (st.top() >= 5){
                    answer += 10 - s;
                    st.top() = st.top() + 1;
                } else {
                    answer += s;
                }
            } else {
                answer += s;
            }
        } else if (s == 10){
            if (!st.empty()){
                st.top() = st.top() + 1;
            } else {
                answer += 1;
            }
        }
    }

    return answer;
}
```

- [문제](https://school.programmers.co.kr/learn/courses/30/lessons/148653?language=cpp)
- 직관적으로 보기위해 스택으로 풀었다.
- 다른 풀이들을 보니 그냥 %10을 연속해서 하는경우도 있는데 그게 더 효율적이고 좋은듯하다.
