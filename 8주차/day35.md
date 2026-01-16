# 코드카타

## 배열의 곱셈

```c++
#include <string>
#include <vector>

using namespace std;

vector<vector<int>> solution(vector<vector<int>> arr1, vector<vector<int>> arr2) {
    vector<vector<int>> answer(arr1.size(), vector<int>(arr2[0].size(), 0));

    for (int i = 0; i < arr1.size(); i++){
        for (int j = 0; j < arr2[0].size(); j++){
            for (int k = 0; k < arr1[0].size(); k++){
                answer[i][j] += arr1[i][k] * arr2[k][j];
            }
        }
    }

    return answer;
}
```

- 배열의 곱셈은 행렬 A가 `m * n`이고 행렬 B가 `n * r`크기일때 가능하다.
  - 앞 행렬의 열의 개수와 뒤 행렬의 행의 개수가 같아야 한다.
  - 결과행렬 C는 `(앞행렬의 행수) * (뒤 행렬의 열수) = m * r`이 된다.
