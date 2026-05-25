![](https://velog.velcdn.com/images/kyu_/post/4677ae21-d5de-48ae-9f0c-d6ab428792e8/image.png)

# 챌린지 반

## 언리얼에서는 상태를 어떻게 사용하고 있나?

- ABP
- Behavior Tree : 따지고보면 이거는 행동 기반이고 상태기반은 아니지만 내부적으로 상태기준이 있기때문에
- State Tree : 상태 트리, 게임플레이 태그를 기준으로
- GAS : 게임플레이 태그 기준으로

## 오토마타

- 계산 이론에서 사용하는 추상적인 계산 모델
- 입력을 받아서 상태를 전환하며, 출력을 생성하는 수학적 기계

## UCLASS

- 언리얼 리플렉션 시스템에 클래스를 등록하는 매크로
- 리플렉션에 등록된 클래스는 엔진이 타입 정보, 프로퍼티/함수 메타데이터, 직렬화, 에디터 노출, GC(가비지 컬렉션) 추적, 네트워크 복제(Replication) 메타데이터
- 언리얼은 C++ "그 자체"만으로는 에디터/블루프린트/복제/직렬화에 필요한 정보를 엔진이 일관되게 처리하기 어렵다. “엔진이 알아야 하는 정보”를 명시하면, 엔진이 자동으로 처리할 수 있다.

### 핵심 포인트

- UObject 계열 (AActor, UActorComponent, UUserWidget)
- 에디터 노출/저장/복제가 필요한 멤버는 일반 C++멤버가 아니라 UPROPERTY()로 선언하는게 기본 전제

# 선형 보간 / 이징

- 선형보간을 자연스럽게 만들기 위해 시징을 어떻게 올바르게 활용하는가?
- 시간 기반으로 보간을 관리 하라
- 기존에는 선형적인 x = lerp(시작, 끝, t) -> t = 경과시간/전체시간 공식을 써서 t가 항상 0에서 1까지 흐르게 만듬
- 선형적인 t를 바로 쓰지않고 이징함수에 통과시킨 후 lerp에 넣으면 t는 원래 0에서 1까지 일정하게 흐르지만, 이징함수에서는 그 흐름을 곡선으로 바꾼다.

# 코드카타

## 가장큰수

```c++
#include <string>
#include <vector>
#include <algorithm>
#include <iostream>

using namespace std;

bool compare(int a, int b){
    string tmpa = to_string(a);
    string tmpb = to_string(b);
    return tmpa+tmpa+tmpa+tmpa > tmpb+tmpb+tmpb+tmpb;
}

string solution(vector<int> numbers) {
    string answer = "";

    sort(numbers.begin(), numbers.end(), compare);

    for (int number : numbers){
        answer += to_string(number);
    }

    if (answer[0] == '0'){
        answer = "0";
    }

    return answer;
}
```

- 수가 1000까지 즉 네자리까지 가능하므로 문자열 비교를할때 4번 더해주었다.
- 0으로 시작하면 숫자로 변환하면 0이므로 예외처리를 해주었다.

## 소수찾기

```c++
#include <string>
#include <vector>
#include <iostream>
#include <algorithm>
#include <cmath>
#include <unordered_set>

using namespace std;

bool isPrime(int tmp){
    if (tmp < 2) return false;

    for (int i = 2; i <= sqrt(tmp); i++){
        if (tmp % i == 0) return false;
    }
    return true;
}

int solution(string numbers) {
    int n = numbers.size();
    unordered_set<int> ss;

    sort(numbers.begin(), numbers.end());

    do{
        string tmp = "";
        for (int i = 1; i <= n; i++){
            int num = stoi(numbers.substr(0, i));
            if (isPrime(num)){
                ss.insert(num);
            }
        }
    } while(next_permutation(numbers.begin(), numbers.end()));

    return ss.size();
}
```

- next_permutation을 최대한 활용했다.
  - 꼭 정렬해서 사용하자
- for문을 1부터 돌면서 substr로 부분문자열들도 다 확인했다.
