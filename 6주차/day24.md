# 팀프로젝트

## 알게된 짜잘이들

### 같은 클래스의 멤버함수 안에서는 클래스명을 다시 명시하지 않아도 된다.

```c++
void GameManager::runBattle() {
    // 같은 클래스의 다른 멤버 함수 호출
    applyBuffItems();  // ✅ 정상 작동
```

### static_cast사용

```c++
int health = static_cast<int>(baseHealth * multiplier);
int attack = static_cast<int>(baseAttack * multiplier);
```

- 그냥 int를 쓰는것보다 명시적으로 static_cast를 써주는게 좋다.
- 타입 안정성이 보장된다.

### 추상 클래스

- Monster클래스에 순수가상함수를 넣어서 추상클래스로 만들었는데 추후에 사용할때 인스턴스를 생성하려고 했다.
- 당연하게도 추상클래스에서는 인스턴스 생성이 안된다..
  - Monster의 하위 클래스로 NormalMonster, BossMonster클래스를 만드는게 좋을 것 같다.

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
