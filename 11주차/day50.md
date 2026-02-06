#

### TObjectPtr

- UObject 멤버 변수를 안전하게 들고 있기 위한 UE5용 포인터 래퍼
- GC/리플렉션/직렬화(세이브, 로드, 에디터 노출) 흐름에 더 잘맞게 UObject 참조를 표현하는데 있다.

### 역할 1. UObject 강한 참조를 멤버로 표현

- UstaticMesh, UTexture2d, UActorComponent, AActor처럼 UObject계열을 멤버로 보관할 때 주로 사용
- 보통은 UPROPERTY()와 같이 써서 엔진이 이 UObject를 참조중임을 추적하게 됨
- 그래야 GC가 수거하면 안되는 객체를 제대로 판단하고, 에디터 Details노출, 직렬화 등도 정상 동작할 가능성이 높다.

### 역할 2. raw pointer(UObject\*)를 대체하는 UE5 표준 스타일

- UObject멤버 포인트는 TObjectPtr로 쓰자

### 어떤 경우에 사용하는가?

- Actor/Component/Asset을 클래스 멤버로 들고 있을때

#### 쓰지 않는경우

- 수명 연장업시 참조만하는 경우 : TWeakObjectPtr
- 에셋을 지금 로드하지않고 경로로만사용 : TSoftObjectPtr, TSoftClassPtr
- UObject가 아닌 일반 객체 : TUniquePtr / TSharedPtr (RAII영역)

# 코드카타

## map과 unordered_map의 차이

- [우박수열정적분](https://school.programmers.co.kr/learn/courses/30/lessons/134239?language=cpp)

```c++
#include <string>
#include <vector>
#include <iostream>
#include <map>

using namespace std;

vector<double> solution(int k, vector<vector<int>> ranges) {
    vector<double> answer;

    // 1 -2
    // k가 몇번만에 1이되느냐
    vector<int> path;
    path.push_back(k);
    int n = 0;
    while (k > 1){
        if (k % 2){
            k *= 3;
            k++;
        } else {
            k /= 2;
        }
        path.push_back(k);
        n++;
    }

    // 우박수열을 보기전에 미리 만개의 크기를 구해놓는게 낫지않나?
    // um[0]은 0과 1사이의 크기
    map<pair<int, int>, double> um;

    // 크기는 어떻게 구함?
    for (int i = 0; i < path.size()-1; i++){
        double maxH = max(path[i], path[i+1]);
        double minH = min(path[i], path[i+1]);

        double value = maxH;
        value -= ((maxH - minH) / 2);
        um[{i, i+1}] = value;
    }

    for (int i = 0; i < ranges.size(); i++){
        int a = ranges[i][0];
        // 애초에 마이너스로 들어가있음
        int b = ranges[i][1];
        double value = 0;
        if (a == 0 && b == 0){
            for (int j = 0; j < path.size()-1; j++){
                value += um[{j, j+1}];
            }
        } else if (a > n+b){
            value = -1.0;
        } else {
            for (int j = a; j < n+b; j++){
                value += um[{j, j+1}];
            }
        }

        answer.push_back(value);
    }

    return answer;
}
```

- 굉장히 특이했던 문제
- 정렬이 필요가 없어서 um을 처음에 unordered_map으로 선언했는데 unordered_map은 키를 pair로 사용할 수 없었다.
  - map은 이진 탐색트리 기반이고 키를 비교할때 < 연산자 사용
  - unordered_map은 해시 테이블 기반, std에서 std::pair에 대한 기본 해시 함수를 제공하지 않아서 키로 쓸 수 없다.
  - 두 map이 기반이 같고 정렬되고 안되고 차이정도인줄 알았는데 기반자체가 다른줄은 몰랐다.
