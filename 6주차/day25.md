## 코드카타

### 소수만들기

```c++
#include <vector>
#include <iostream>
#include <cmath>
using namespace std;

bool isPrime(int num){
    if (num < 2){
        return false;
    }
    if (num == 2){
        return true;
    }
    for (int i = 2; i <= sqrt(num); i++){
        if (num % i == 0){
            return false;
        }
    }
    return true;
}

int solution(vector<int> nums) {
    int answer = 0;
    for (int i = 0; i < nums.size() - 2; i++){
        for (int j = i+1; j < nums.size()-1; j++){
            for (int k = j + 1; k < nums.size(); k++){
                int sum = nums[i] + nums[j] + nums[k];
                if (isPrime(sum)){
                    answer++;
                }
            }
        }
    }



    return answer;
}
```

- <cmath>헤더에있는 sqrt를 빼먹지 말자
- C++에서 조합 만드는방법을 배워서 써먹으려고 했는데 생각보다 복잡해보여서 우선은 삼중for문으로 해결했다.
