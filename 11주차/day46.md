# 코드카타

## XOR연산

- [테이블 해시 함수](https://school.programmers.co.kr/learn/courses/30/lessons/147354)

```c++
#include <string>
#include <vector>
#include <algorithm>
#include <iostream>

using namespace std;

int solution(vector<vector<int>> data, int col, int row_begin, int row_end) {
    int answer = -1;

    sort(data.begin(), data.end(), [col](auto& a, auto& b){
        if (a[col-1] != b[col-1]) return a[col-1] < b[col-1];
        return a[0] > b[0];
    });

    for (int i = row_begin-1; i <= row_end-1; i++){
        vector<int> target = data[i];
        int sumV = 0;
        for (int t : target){
            sumV += t % (i+1);
        }

        if (answer == -1){
            answer = sumV;
        } else {
            answer ^= sumV;
        }

    }
    return answer;
}
```

- XOR연산은 ^로 간단하게 할 수 있다는 것을 배웠다.

## 중복계산 ㄴ

- [시소짝꿍](https://school.programmers.co.kr/learn/courses/30/lessons/152996)

```c++
#include <string>
#include <vector>
#include <algorithm>
#include <unordered_map>
#include <iostream>
using namespace std;

long long solution(vector<int> weights) {
    long long answer = 0;
    unordered_map<int, int> um;
    sort(weights.begin(), weights.end());

    for (int i = 0; i < weights.size(); i++){
        int target = weights[i];
        if (um.count(target)) answer += um[target];
        if ((target*2)%3 == 0 && um.count(target*2/3)) answer += um[target*2/3];
        if (target%2 == 0 && um.count(target*1/2)) answer += um[target*1/2];
        if ((target*3)%4 == 0 && um.count(target*3/4)) answer += um[target*3/4];
        um[target]++;
    }

    return answer;
}
```

- 완탐으로 돌리기에는 입력이 너무 크다.
- 아이디어는 맞는데 계속 틀려서 애먹었던 문제
- 오름차순 정렬을 하고, unordered_map에 넣어주면서 계산을해서 중복 계산을 방지했다.
- um[실수값]이 되버리면 소수점 부분을 버리고 계산이 이상해지므로 나머지 계산으로 0인경우만 계산을 했다.
