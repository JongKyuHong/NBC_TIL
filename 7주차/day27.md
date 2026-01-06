## 코드카타

### 기사단원의 무기

```c++
#include <string>
#include <vector>
#include <cmath>

using namespace std;

int prime(int num){
    int ans = 0;
    for (int i = 1; i <= sqrt(num); i++){
        if (num % i == 0){
            ans++;
        }
    }
    return ans+1;
}

int solution(int number, int limit, int power) {
    int answer = 0;

    for (int i = 1; i <= number; i++){
        int res = prime(i);
        if (res > limit){
            answer+=power;
        } else {
            answer += res;
        }

    }

    return answer;
}
```

- 기존 방법은 그냥 소수를 구하는 방법을 사용했는데 number가 10만이기때문에 최악의 경우 100억번 연산이 필요했다.

```c++
#include <string>
#include <vector>

using namespace std;

int solution(int number, int limit, int power) {
    vector<int> CountPrime(number+1, 0);

    for (int i = 1; i <= number; i++){
        for (int j = i; j <= number; j+=i){
            CountPrime[j]++;
        }
    }

    int answer = 0;
    for (int i = 1; i <= number; i++){
        if (CountPrime[i] > limit){
            answer += power;
        } else {
            answer += CountPrime[i];
        }
    }

    return answer;
}
```

- 에라토스테네스의 체 방식으로 코드를 개선하여 풀게되었다.
