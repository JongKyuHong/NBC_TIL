## 코드카타

### set을 vector로 변환

- 두개 뽑아서 더하기 문제

```c++
#include <string>
#include <vector>
#include <unordered_set>
#include <algorithm>

using namespace std;

vector<int> solution(vector<int> numbers) {
    unordered_set<int> us;
    for (int i = 0; i < numbers.size()-1; i++){
        for (int j = i+1; j < numbers.size(); j++){
            us.insert(numbers[i]+numbers[j]);
        }
    }

    vector<int> answer(us.begin(), us.end());

    sort(answer.begin(), answer.end());

    return answer;
}
```

- vector를 선언할때 셋의 이터레이터를 넘겨주어 셋을 안정적으로 벡터로 변환할 수 있다.

### 람다 함수 정렬에서 외부변수 사용하기

- 문자열내마음대로정렬하기

```c++
#include <string>
#include <vector>
#include <algorithm>

using namespace std;

vector<string> solution(vector<string> strings, int n) {
    vector<string> answer;

    sort(strings.begin(), strings.end(), [n](const auto& a, const auto& b){
        if (a[n] == b[n]){
            return a < b;
        }
        return a[n] < b[n];
    });

    return strings;
}
```
