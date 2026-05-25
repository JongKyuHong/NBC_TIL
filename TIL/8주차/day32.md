![](https://velog.velcdn.com/images/kyu_/post/bce41dfe-0450-4a34-b3a6-c8780c55aa4d/image.png)
![](https://velog.velcdn.com/images/kyu_/post/9fafff5a-0521-49dc-898a-e893567fe650/image.png)

# 언리얼C++강의

## 3-1 강의

## 인터페이스 이해하기

- 인터페이스 : 클래스가 반드시 구현해야하는 함수 목록만을 미리 정의해두고 실제 함수의 동작은 해당 클래스를 상속받거나 구현하는 쪽에서 자유롭게 작성할 수 있는 일종의 계약서

### 상속과의 차이

- 상속은 부모의 실제 구현을 가져다 쓰지만 인터페이스는 함수의 틀만 빌리고 그 안의 코드는 하위에서 직접 만들어서 사용

### 인터페이스의 장점

- 결합도 감소
  - 클래스간 구현 내용을 공유하지않고 필요한 함수 목록만 약속하므로 클래스 간 의존도가 낮다.
  - 다른 클래스 내부가 어떻게 돌아가는지 몰라도, 이 함수를 이렇게 호출하면 된다 정도만 알면 됨
- 확장성 향상
- 다형성 극대화

  - `TArray<IItemInterface*> Items`와 같은 인터페이스 포인터 배열로 관리하면, 아이템 종류가 무엇이든 같은 함수를 호출하여 다룰 수 있다.

```c++
// 인터페이스를 UObject 시스템에서 사용하기 위한 기본 매크로
UINTERFACE(MinimalAPI)
class UItemInterface : public UInterface
{
    GENERATED_BODY()
};

// 실제 C++ 레벨에서 사용할 함수 원형(시그니처)를 정의
class SPARTAPROJECT_API IItemInterface
{
    GENERATED_BODY()

public:
    // 플레이어가 이 아이템의 범위에 들어왔을 때 호출
    virtual void OnItemOverlap(AActor* OverlapActor) = 0;
    // 플레이어가 이 아이템의 범위를 벗어났을 때 호출
    virtual void OnItemEndOverlap(AActor* OverlapActor) = 0;
    // 아이템이 사용되었을 때 호출
    virtual void ActivateItem(AActor* Activator) = 0;
    // 이 아이템의 유형(타입)을 반환 (예: "Coin", "Mine" 등)
    virtual FName GetItemType() const = 0;
};
```

- 순수 가상함수 처리
- UINTERFACE 매크로필수
- UInterface를 상속

아이템 인터페이스 구현 -> 아이템 부모 클래스 구현 -> 코인 부모 클래스 구현 -> 빅,스몰코인 구현

## 3-2. 충돌 이벤트

- 충돌 영역

  - SphereComponent나 BoxComponent사용
  - Overlap이벤트 발생 이를 감지

- Overlap이벤트 vs Hit이벤트
  - Overlap은 단순 겹침
  - Hit은 실제 물리 충돌( ex: 탄환이 벽에 부딪힘)

## 3-3. 아이템 스폰 레벨 데이터 관리

### Item Data 구조체

- 어떤 아이템이 %확률로 스폰되는것을 하드코딩하기는 빡시다 그래서 구조체 사용
- CSV나 JSON파일로 관리 가능

### 아이템 스폰

랜덤으로 위치값 지정해서 아이템을 스폰해주었다.
레벨마다 각기 다른 확률 데이터를 적용

## 3-4. 캐릭터 체력 및 점수 관리 시스템

### 캐릭터 체력 시스템 구현하기

- PlayerState를 쓰지 않는 이유
  - PlayerState는 주로 멀티플레이 환경에서 각 플레이어 간 데이터 동기화를 위해 사용
    - 예를들어 킬/데스 카운트처럼 서버와 클라이언트 모두가 공통으로 확인해야하는 정보
  - 하지만 싱글플레이는 이런 동기화가 필요가 없으므로 캐릭터 클래스 자체에 체력이나 스코어변수를 넣어서 관리해도 됨

### 점수 관리 시스템 구현하기

- GameMode와 GameState의 연계 이해하기

  - 게임의 전역 정보를 유지, 필요에 따라 멀티플레이 환경에서 서버와 클라이언트 간에 동기화하는 역할을 함

- GameMode

  - 게임의 규칙을 정의하고 관리
  - 어떤 캐릭터를 스폰할지, 플레이어가 사망했을 때 어떻게 처리할지 결정
  - 멀티플레이에서는 서버 전용으로 동작(클라이언트에 존재하지 않음)

- GameState
  - 게임 플레이 전반에서 공유되어야하는 전역상태 의미
  - 기본적으로 게임당 하나 존재, 엔진 내부에서 데이터 동기화를 고려해 설계되었기에 전역 데이터 관리용으로 적합
  - 대표적으로 점수, 남은 시간, 현재 게임 단계, 스폰된 오브젝트의 총 개수등을 저장
  - 멀티플레이에서는 서버가 관리하고, 클라이언트는 이를 자동으로 동기화 받아볼 수 있음

## 3-5. 게임 루프 설계를 통한 게임 흐름 제어

### GameState와 GameMode

- GameMode를 쓰는 이유?
  - 서버 전용 로직
  - 클라이언트는 접근 불가, 클라이언트도 알아야 하는 정보는 두면 안됨(복잡해짐)
  - 중요 규칙 로직만 GameMode에 두고, 서버 - 클라이언트가 공통으로 알아야하는건 GameState에 둠
- GameState를 쓰는 이유?
  - 모든 플레이어가 공유해야 하는 상태를 담는 클래스 보통 전역 상태가 필요할때 GameState활용
  - 게임이 시작될때 서버에서 생성되고, 클라이언트는 이를 복제받아서 똑같은 정보를 읽을 수 있다.

### GameInstance란?

- 레벨 전환 시 GameState, GameMode와 같은 기본 클래스를 비롯해, 맵 내에서 생성된 대부분의 객체가 처음부터 다시 생성된다.
- 어떤 경우에는 이전 레벨의 상태를 모든 레벨에 걸쳐서 유지하고 싶을때 두가지 방법을 사용 GameInstace, Seamless Travel

- Game Instance
  - 프로젝트가 시작될 때부터 애플리케이션이 완전히 종료될 때까지 유일하게 계속 살아있는 객체
  - 맵이 전환되어도 파괴되지 않으므로 여기서 전역 데이터 관리 가능
- Seamless Travel
  - 멀티 플레이 환경에서 주로 사용되는 레벨 전환 방식으로, GameState/PlayerController등을 파괴하지 않고 그대로 다음 맵으로 넘어가는 기능
  - Seamless Travel을 사용하면 대부분의 객체를 유지할 수 있지만, 설정과 로직이 조금 더 복잡하므로, 싱글 플레이 전용 간단 프로젝트라면 GameInstace를 사용하기가 쉽다.

# 코드카타

## 개인정보수집유효기간문제

```c++
#include <string>
#include <vector>
#include <unordered_map>

using namespace std;

int calculateDate(string& date){
    int year = stoi(date.substr(0, 4));
    int month = stoi(date.substr(5, 2));
    int day = stoi(date.substr(8, 2));

    int answer = year*12 + month-1;
    answer *= 28;
    answer += day;
    return answer;
}

vector<int> solution(string today, vector<string> terms, vector<string> privacies) {
    vector<int> answer;
    unordered_map<char, int> monthTerm;

    for (string& term : terms){
        char t = term[0];
        int month = stoi(term.substr(2));
        monthTerm[t] = month;
    }

    int calToday = calculateDate(today);

    for (int i = 0; i < privacies.size(); i++){
        string date = privacies[i].substr(0, 10);
        char termType = privacies[i][11];

        int start = calculateDate(date);
        int end = start + monthTerm[termType] * 28;

        if (end <= calToday){
            answer.push_back(i+1);
        }
    }
    return answer;
}
```

- substr활용(시작위치, 길이)
- 모든 월이 28일로 고정되어서 쉽게 풀었다.

# 총 정리

1. 인터페이스

- 클래스가 반드시 구현해야하는 함수 목록(계약서)
- 상속과 달리 함수의 틀만 약속하므로 결합도 낮아지고 확장성 좋다.
- 인터페이스 포인터 배열로 다형성도 챙길 수 있음

2. 충돌 이벤트

- Overlap은 감지 Hit은 물리적 충돌

3. 아이템 구조체

- 구조체로 아이템 종류/확률을 관리
- CSV, JSON을 사용할수도 있음

4. PlayerState

- 멀티플레이에서는 자주 사용한다.
- 동기화가 필요하기때문

5. GameMode

- 규칙/스폰/사망처리 등 게임 규칙
- 서버 전용
- 클라이언트도 알아야하면 GameState에 두자

6. GameInstance

- 레벨 전환 시 대부분의 객체가 재생성되므로 사용
- 전역데이터를 두고 사용하면 된다.
- 멀티에서는 Seamless Travel 고려

7. 코드카타

- substr사용법 익히기
