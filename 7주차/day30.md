# 코드카타

## 1.

- 체육복

```c++
#include <string>
#include <vector>
#include <unordered_map>
#include <algorithm>

using namespace std;

bool algo(unordered_map<int, int>& um, vector<int>& reserve, int l){
    auto findk3 = um.find(l);
    if (findk3 == um.end()){
        auto it3 = find(reserve.begin(), reserve.end(), l);
        if (it3 != reserve.end()){
            um.insert({l, 1});
            return true;
        }
    }
    return false;
}


int solution(int n, vector<int> lost, vector<int> reserve) {
    int answer = 0;
    unordered_map<int, int> um;
    sort(lost.begin(), lost.end());

    for (int r : reserve){
        auto findk3 = um.find(r);
        if (findk3 == um.end()){
            auto it3 = find(lost.begin(), lost.end(), r);
            if (it3 != lost.end()){
                um.insert({r, 1});
            }
        }
    }

    for (int l : lost){
        if (um.find(l) != um.end()){
            continue;
        }
        if (algo(um, reserve, l)){
            continue;
        }
        if (algo(um, reserve, l-1)){
            continue;
        }
        if (algo(um, reserve, l+1)){
            continue;
        }

        answer++;
    }

    return n-answer;
}
```

- 본래코드는 reserve에서 바로 지우는 방식이 기억이 안나서 이렇게 um이라고 따로 처리하는 map을 사용했다.

```c++
#include <string>
#include <vector>
#include <algorithm>

using namespace std;

int solution(int n, vector<int> lost, vector<int> reserve) {
    int answer = 0;
    sort(lost.begin(), lost.end());

    for (int& l : lost){
        auto it = find(reserve.begin(), reserve.end(), l);
        if (it != reserve.end()){
            reserve.erase(it);
            l = 0;
        }
    }

    for (int l : lost){
        if (l == 0){
            continue;
        }
        auto it = find(reserve.begin(), reserve.end(), l-1);
        if (it != reserve.end()){
            reserve.erase(it);
            continue;
        }
        auto it2 = find(reserve.begin(), reserve.end(), l+1);
        if (it2 != reserve.end()){
            reserve.erase(it2);
            continue;
        }
        answer++;
    }

    return n-answer;
}
```

- 찾아보니 이런식으로 it는 그냥 erase의 인자로 넘겨주면 삭제할 수 있더라..

```c++
for (int& l : lost){
    auto it = find(reserve.begin(), reserve.end(), l);
    if (it != reserve.end()){
        reserve.erase(it);
        l = 0;
    }
}
```

- 이 구문도 신기했다. &로 인해 원본에 접근이 가능하지 처리한 사람은 0으로 처리해버리기
