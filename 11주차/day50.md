# 8번과제 해설 코드 분석

## 목표

- 프로젝트의 전반적인 구조, 코드 분석을 통해 이번 팀프로젝트에 활용할 계획
- 특히 스텟관리 부분에서 GAS를 쓸 수 없으므로 어떤식으로 구현하는게 나을지 튜터님의 코드를 보고 활용하려고 한다.

## 문서

### 에셋 네이밍

- Content/ 아래에 바로 에셋폴더를 넣지말고 Content/프로젝트명/ 아래에 넣자
- 글로벌 네임스페이스를 더럽히지 말자

### 클래스별 용도

- GameInstance : 레벨 전환에도 유지, 전역 데이터 보관
- GameMode : 게임 규칙 정의
- GameState : GameMode에서 생성, 경과 시간, 팀 스코어, 매치 상태, PlayerState 배열 보유
- PlayerController : 입력 수신/처리, Possess, HUD/UI관리, 카메라 제어
- PlayerState : 개별 플레이어 공개 정보, 이름/스코어/팀/Ping 등

### Format

- TEXT보다는 FString::Format사용

```c++
FString Msg = FString::Format(
    TEXT("{0}: {1}레벨, 데미지 {2}"),
    { PlayerName, Level, Damage }  // FString, int, float 혼합
);
```

### Best Practice

- FSM(상태 머신) : 게임 흐름을 `Begin -> Progress -> End -> Intermission` 상태로 분리, 각 상태 클래스가 자신의 로직만 담당하여 유지보수가 쉬움
- GameMode/State분리 : 규칙, 공유 정보 역할을 명확하게 나누기
- 데이터 테이블 개선 : 한 엑셀 파일에서 대량 편집 가능하도록 스키마 변경
- 버프 컴포넌트 : 캐릭터에 붙이는 컴포넌트로 버프 관리, 중첩/해제 로직이 분리되어 새 버프 추가가 간단
- BindWidget 메타 태그 : `meta = (BindWidget)`으로 위젯 자동 바인딩

## 분석

### GameInstance

#### Init()

- GameInstance가 준비될때 호출
- 생성자는 객체가 만들어지는 순간이에 돌고, Init()은 서브 시스템 초기화, 데이터 테이블 로드같은 전역 서비스 준비를 둠

### UGameInstanceSubsystem

- GameInstance에 붙어서 전역 서비스(매니저) 객체
- GameInstance가 비대해지는것을 막고, 기능을 모듈(Subsystem)으로 분리하는 역할

#### 사용처

- 세이브/로드 관리
- 데이터 테이블/데이터 에셋 로딩 및 캐싱
- 로그, 통계, 설정(옵션) 관리
- 전역 UI흐름(로딩 화면 같은) 관리
- 온라인 세션/매치메이킹 관리

# 개념

## TObjectPtr

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

## 디펜스게임

- [디펜스게임](https://school.programmers.co.kr/learn/courses/30/lessons/142085)

```c++
#include <string>
#include <vector>
#include <queue>
#include <iostream>

using namespace std;

int solution(int n, int k, vector<int> enemy) {
    int answer = 0;

    priority_queue<int> pq;
    int remain = n;
    int i = 0;
    while (i < enemy.size()){
        pq.push(enemy[i]);
        remain -= enemy[i];
        if (remain < 0){
            if (k <= 0){
                break;
            }
            k -= 1;
            remain += pq.top();
            pq.pop();
        }
        i++;
    }

    return i;
}
```

- 무적권이 있고 무적권을 적절하게 사용해서 적을 몇명 막을수있나 이런 문제였다.
- if (remain < 0) 이부분, 병사가 몇명 남았느지 확인하는 부분에서 remain <= 0을 해서 만약 병사 수가 딱 0명이 되었을때도 실제로는 해당 라운드를 클리어 한거지만 <=라고 조건을 걸어서 클리어하지 못한 상태로 처리가 되고있었다.
  - < 등호를 빼서 해결했는데 이 부분 찾는데만 20분넘게 걸린듯
  - 막히면 뇌를 좀 리프레시하고 다시 문제를 처음만난것처럼 새롭게 시작하는게 중요한거같다.
