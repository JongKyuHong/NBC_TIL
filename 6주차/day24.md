## 코드카타

### 최대값 찾기

- 기본 사용법
  <algorithm> 헤더에 있고 두값을 비교할때 사용한다.

```c++
#include <algorithm>

int maxV = max(a, b);
```

- 여러값 비교
  {}중괄호를 통해서 여러값을 비교한다.

```c++
#include <algorithm>

int maxV = max({a, b, c});
```

- 벡터/배열에서 최대값 찾기

max_element와 이터레이터를 통해서 찾는다.

```c++
#include <algorithm>
#include <vector>

vector<int> v = {3, 1, 4, 1, 5, 9, 2};

int maxV = max_element(v.begin(), v.end());
```
