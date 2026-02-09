# 코드카타

## 두 원 사이의 정수 쌍

```c++
#include <string>
#include <vector>
#include <cmath>

using namespace std;

long long solution(int r1, int r2) {
    long long answer = 0;

    // r1의 반지름값보다 같거나 크고, r2의 반지름보다 작거나 같은 정수쌍 나열임
    // y값이 1부터 시작하고 최대 r2의 반지름 만큼 확인?

    long long cnt = 0;

    for (int y = 1; y <= r2; y++){
        for (int x = 0; x <= r2; x++){
            if (pow(r1, 2) <= pow(y, 2) + pow(x, 2) && pow(r2 ,2) >= pow(y, 2) + pow(x, 2)){
                cnt++;
            }
        }
    }

    return cnt*4;
}

```

- 처음에는 완전탐색을 돌렸는데 당연하게도 입력이 100만이라 시간초과가 났다./

```c++
#include <string>
#include <vector>
#include <cmath>
#include <iostream>

using namespace std;

long long solution(int r1, int r2) {
    long long answer = 0;

    long long cnt = 0;

    for (int x = 1; x <= r2; x++){
        // 원의 경계 안쪽으로 들어와야 해서 floor
        long long maxY = floor(sqrt((long long)r2 * r2 - (long long)x * x));

        // r1원의 경계 밖으로 나와야 해서 ceil
        long long minY = 0;
        if (x < r1){
            minY = ceil(sqrt((long long)r1 * r1 - (long long)x * x));
        }

        answer += maxY - minY + 1;
    }
    return answer*4;
}
```
