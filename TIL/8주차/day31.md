![](https://velog.velcdn.com/images/kyu_/post/875f4548-eb18-4921-9b15-f1162e35a8c2/image.png)

# 언리얼 C++

## 기본적인 시퀀스

1. 캐릭터 구현
2. 캐릭터 입력 매핑 구현
3. 캐릭터 기본 동작 구현 (WASD 이동, 점프, 카메라 회전, 스프린트)
4. 캐릭터 애니메이션 적용 (Idle, Walk, Jump, Sprint)
5. 아이템 설계하기
6. 아이템 상호작용
7. 아이템 랜덤 스폰 및 레벨 별 각 아이템 개수 설정
8. 캐릭터 데미지 및 아이템 점수 관리 시스템
9. 웨이브 시스템을 통한 게임 흐름 제어
10. HUD에 실시간 정보 반영
11. 게임 흐름에 맞춘 메뉴 UI 구현
12. UI 애니메이션 효과 및 3D 위젯 UI 구현
13. 파티클 효과 적용(지뢰 폭발, 아이템 습득)
14. 사운드 효과 적용(지뢰폭발사운드, 아이템 습득, 발걸음)
15. 프로젝트 배포

## 2-1강

### GameMode이해하기

- 게임의 전반적인 규칙과 흐름을 총괄, 일종의 컨트롤 타워
- 싱글 게임의 경우 서버나 클라이언트 개념이 나뉘지않기 때문에 로컬에서 게임모드가 모든 것을 제어한다.

#### 주요책임

- Pawn/Character 스폰
  - DefaultPawnClass로 지정한 Pawn을 자동으로 스폰
  - PlayerController 연결
- 게임 규칙관리
- GameState/PlayerState사용

### GameMode

- Base랑 그냥이 나뉜다.
- Base는 응애용 , 멀티플레이 지원안됨, 작은 싱글게임
- 그냥은 Base를 상속받아 사용, 대부분에 이거

### Pawn/Character

- Pawn

  - 플레이어 혹은 AI가 소유할 수 있는 가장 최상위 클래스
  - 이동, 충돌, 중력, 네트워크 이동을 위한 기능이 기본적으로 포함되지 않음
    - 보행에 필요한 캡슐 콜리전, 중력, 지형따라가기 기능이 없음
    - 사람 캐릭터를 처음부터 이걸로 만들기에는 부담

- Character
  - Pawn을 상속받아 만들어진 자식 클래스
  - 캐릭터를 만드는데 필요한 것들이 모두 담겨있음
  - UCharacterMovementComponent를 포함

### 메모

1강

GameMode 클래스 - 총괄 관리자

1. 플레이어 캐릭터 - Pawn 클래스 or Character 클래스
2. PlayerController 클래스 - 캐릭터에 빙의
3. 게임 규칙 관리 - 로직 (함수)
4. GameState 클래스 - 게임 전역 데이터 - 점수
   PlayerState 클래스 - 개별 캐릭터마다의 데이터

GameModeBase와 Gamemode의 차이는 base는 조금 더 초급분들이 사용하면 좋다. 멀티플레이 로직이 없다고 보면 됨, 간단한게임만들기 적합하다. 엔진자체에서 제공하는 기능이 그렇게 많지 않다.

## 2-2강

### PlayerController

- 마우스, 키보드, 게임패드 등에서 입력을 받으면 그 입력을 해석해서 캐릭터나 다른 오브젝트에 동작을 명령
- 언리얼의 중요한 철학 `플레이어 입력은 PlayerController에서 처리한다`

### UFUNCTION()

- 입력 바인딩 함수가 언리얼 엔진 리플렉션 시스템에 연동되어야 하기때문에 UFUNCTION()을 붙이지 않으면 바인딩에 실패할 수 있다.

### 메모

Player Controller

1. 입력 처리 - Enhanced Input System - 언리얼 엔진 5
2. 카메라 제어 로직
3. UI와의 상호작용
4. Possess - 빙의 / UnPossess

Enhanced Input System

- 사람 전용 IMC
- 자동차 전용 IMC
- Input Mapping System(IMC) : 스위치
  - IA 들을 총괄해서 관리
- Input Action (IA) - 추상적인 행동(전선)

  - 점프 -> IA_JUMP
  - 마우스 회전 -> IA_Look
  - 이동 -> IA_Move

- 캐릭터 행동

1. 이동 WASD
2. 마우스 회전
3. 점프
4. 스프린트 shift

## 2-3강

### Character클래스에 액션 바인딩 추가

1. PlayerController는 IMC를 활성화
2. 해당 IMC에는 IA로 작성한 UInputAction들이 매핑되어있음
3. 캐릭터가 SetupPlayerInputComponent()함수를 통해 각 액션이 발생했을 때 어떤 함수를 실행할지 등록
4. 등록한 함수들이 실제로 움직이거나 점프하는등 캐릭터 동작 수행

### 메모

1. GameMode - Character, PlayerController
2. Character - GameMode에 연결 - 월드에 스폰되도록
3. Player Controller - 사용자의 입력과 Character사이에서 연결
4. IMC, IA 입력 매핑
   IMC활성화
   Local Player -> Local Player Subsystem -> IMC 활성화
   Local Player = 사용자

5. 사용자가 A
6. Player Controller A인식
7. Local Player Subsystem - A가 무슨 뜻이야??
8. 똉떙 동작임.
9. PlayerController -> Character A는 이런뜻이니까 거기에 맞는 행동 함수를 호출해라
10. Character A에 맞는 함수를 실행

## 복습

### 1-4강

#### 컴포넌트란?

- Actor가 어떤 역할을 하거나 특정 속성을 갖도록 만들어주는 부품

#### 루트 컴포넌트/Scene Component

- 모든 Actor는 루트 컴포넌트를 가져야 한다.
- 액터의 트랜스폼을 정의하는 최상위 컴포넌트, 모든 하위컴포넌트가 이를 기준으로 트랜스폼이 계산된다.
- 보통은 Scene Component를 루트 컴포넌트로 설정하여 액터의 트랜스폼을 관리
  - Scene Component는 모든 트랜스폼 속성만 가지는 비시각적인 컴포넌트
  - 직접적인 시각적인 출력은 가지지 않지만, 다른 하위 컴포넌트를 관리하는 기준점 역할

### 1-5강

#### 액터의 라이프 사이클

- 알아야 하는 이유

1. 초기화 시점 결정
2. 성능 관리
3. 리소스 정리

- 주요 함수

1. 생성자
2. PostInitializeComponents()
3. BeginPlay()
4. Tick(float DeltaTime)
5. Destroyed()
6. EndPlay()

### 1-7강

#### 리플렉션 시스템 이해하기

1. Blueprint(시각적 스크립팅) 이해

   블루프린트는 언리얼엔진에서 제공하는 시각적 스크립팅 도구, 노드(블록)를 연결하여 게임 로직을 작성한다.
   장점 : 아이디어를 빠르게 검증할때 장점, 프로그래밍언어에 익숙치 않아도 쉽게 배움
   단점 : 노드수가많을수록 그래프가 복잡해져 가독성과 유지보수가 좋지않고, 내부적으로 추가작업이 필요하므로 물리연산 AI같은 높은 성능이 필요한 시스템에서는 병목이 될 수 있다.

2. C++ 이해
   장점 : 엔진 코어까지 직접 수정 가능, 복잡하고 성능이 중요한 게임 로직을 빠르고 최적화된 방식으로 구현 가능, 라이브러리 자유롭게 사용, 대규모 프로젝트 적합, 포인터 템플릿같은 C++언어적 기능 사용가능
   단점 : C++코드 수정시 적용하는게 오래걸려서 번거러움

3. Blueprint와 C++의 상호 보완적 관계

- 실무에서는 같이 사용, 서로의 장점만
  - Blueprint : UI제작, 간단한 이벤트 처리, 시각적 연출 -> 빠른 프로토타이핑과 직관적인 로직 작성에 사용
  - C++ : 높은 성능이 필요한곳, 게임 로직, 수학등

#### 리플렉션이란???

- C++클래스의 변수 및 함수 정보를 엔진 내부의 메타데이터 형태로 저장하고, 이를 에디터나 블루프린트에서 활용할 수 있게 만들어주는 기술
  - C++클래스에 있는 여러 멤버를 Reflection해 에디터와 블루프린트에서 직접 설정, 호출이 가능하도록 함
  - C++ 로직의 뼈대를 디자이너나 다른 팀원들이 에디터에서 작관적으로 조정할 수 있다.
  - 매개변수를 코드에서만 변경하는게 아니라 에디터에서 바로 조정가능, 반복테스트를 빠르게 할 수 있다.

#### 등록 방법

- `#include "이름.generated.h"
  - 언리얼 엔진이 자동으로 생성하는 헤더파일, 클래스의 리플렉션 및 엔진 통합에 필요한 코드가 들어있다.
  - `반드시 헤더파일의 마지막에`
- `UCLASS()`

  - 해당 클래스를 언리얼 엔진의 리플렉션 시스템에 등록한다는 의미
  - Blueprintable : 블루프린트에서 상속 가능한 클래스로 만듬
  - NotBlueprintable : 상속할수 없도록
  - BlueprintType : 블루프린트에서 변수나 참조로 사용할 수 있게, 상속은 허용되지않고 참조만

- `GENERATED_BODY()`
  - 언리얼 코드 생성 도구가 사용하는 코드를 삽입하는 역할
  - 클래스 내부에 필요한 리플렉션 정보를 자동으로 생성함

#### 변수에 등록

- `UPROPERTY()`

1. 편집 가능 범위 지정자
   - `VisibleAnywhere` : 읽기전용으로 표시, 수정은 불가
   - `EditAnywhere` : 클래스 기본값, 인스턴스 모두에서 수정 가능
   - `EditDefaultsOnly` : 클래스 기본값에서만 수정 가능
   - `EditInstanceOnly` : 인스턴스에서만 수정 가능
2. Blueprint 접근성 지정자
   - `BlueprintReadWrite` : Blueprint그래프에서 Getter/Setter로 값을 읽거나 쓸 수 있다.
   - `BlueprintReadOnly` : Blueprint그래프에서 Getter 핀만 노출되어, 읽기만 가능
3. Category 지정자
   - 카테고리별로 묶임
   - 세부정보에서 깔끔하게 볼수있음
4. 메타 옵션 지정자
   - `meta(ClampMin="0.0")` : 에디터에서 변수 입력 시 최소값을 제한할 수 있음
   - `meta=(AllowPrivateAccess="true")` : 해당 멤버가 private으로 선언되어 있어도, 에디터나 Blueprint에서 접근할 수 있도록 허용

- 만약 UPROPERTY()만 있고, 추가 지정자를 하나도 주지않으면?
  - 엔진 리플렉션 시스템에 등록은 되지만 에디터에서 노출되지는 않음
  - 리플렉션에 등록만 되어있어도 가비지 컬렉션(메모리 관리)과 직렬화(세이브/로드)같은 엔진 내부 기능이 작동할 수 있다.

#### 함수에 등록

함수 또한 리플렉션 시스템에 등록할 수 있다. 이렇게 하면 복잡한 C++로직을 Blueprint에서 간단한 노드로 불러와 제어할 수 있으므로 작업 효율이 높아진다.

- `UFUNCTION()`

1. Blueprint관련 지정자
   - `BlueprintCallable` : Blueprint 이벤트 그래프(노드)에서 호출 가능한 함수로 만듬
   - `BlueprintPure` : Getter 역할만 수행한다.
   - `BlueprintImplementableEvent` : 함수를 선언만 C++에 있고, 구현은 블루프린트에서 하도록함 C++코드에서는 함수 이름만 정의하고, 실제 동작은 BlueprintEvent Graph안에서 이벤트 노드처럼 구현됩니다.

- 지정자를 하나도 쓰지 않았다면??
  - UPROPERTY()와 마찬가지로, 함수가 언리얼 리플렉션에 등록되긴 하지만, 특별히 Blueprint에 노출되지는 않음

## 궁금한거

### _리플렉션 시스템_

- UE에서 관리할 수 있게 `직렬화`
- GC에서 추적할 수 있게 `직렬화`
- 직렬화하는것이 포인트인듯

### 1. 변수나 클래스 함수를 사용할때 무조건 리플렉션 시스템에 등록하는게 좋은지??

- 이 값/함수가 UE시스템과 상호작용 해야하는가?? 로 결정하는게 낫다.
- 에디터에서 조정, 블루프린트 접근, 저장/로드(직렬화), 네트워크 리플리케이션, GC 추적이 필요하면 등록이 사실상 필수

### 2. 리플렉션 시스템에 변수정도는 많이 등록해도 성능적으로 문제가 생기지는 않는지??

- 리플렉션 메타데이터가 늘어나는 비용(메모리, UHT 처리/빌드 시간, 에디터 로딩 체감)이 먼저 현실적으로 나타남
- 추적해야 할 레퍼런스(UPROPERTY로 노출된 UObject 참조)가 많아질수록 GC 작업량에 영향을 줄 수 있다.

### 3. 만약에 등록하지 않는게 나은 경우가 있다면 어떤 경우인지

- 엔진이 알면 오히려 위험한 값 : 내부 상태, 불변식
- 직렬화 되면 안되는 값 : 저장되면 곤란한 캐시/런타임 계산 결과는 아예 미등록이 편할수있다.

### 4. 리플렉션에 등록해놓은 함수나 변수들은 동적 메모리해제, 댕글링 포인터같은 메모리 누수로 부터 자유로운건지??

- 일반 C++ 동적 메모리를 자동으로 관리해주는것이 아니라 기본적으로 UObject 세계를 대상으로 하는 수명 관리
- 그래서 리플렉션에 등록했다고 무조건 해결은 아니고 C++에서 관리해주어야함

# 코드카타

## 1. 둘만의 암호

```c++
#include <string>
#include <vector>
#include <iostream>

using namespace std;

string solution(string s, string skip, int index) {
    string answer = "";
    vector<char> vec(26, 'a');
    vector<bool> isSkip(26, false);
    for (int i = 0; i < 26; i++){
        vec[i] = 'a' + i;
    }

    for (int i = 0; i < 26; i++){
        if (skip.find(vec[i]) != string::npos){
            isSkip[i] = true;
        }
    }

    for (int i = 0; i < s.length(); i++){
        char current = s[i];
        int tmp = 0;
        while (tmp < index){
            current++;
            if (current > 'z'){
                current = 'a';
            }
            if (!isSkip[current-'a']){
                tmp++;
            }
        }
        answer += current;
    }

    return answer;
}
```

- 문자를 다루는데 굉장히 유용한 문제였던것 같다.
  - char형 current를 그냥 current++해서 사용한다던지
  - char형 벡터 vec에 'a'+i : 즉 문자더하기 int를 해서 문자로 넣는다던지
- isSkip이라는 skip에 들어가는 알파벳들을 빠르게 판단할수있는 벡터를 하나 만들어서 사용했다.
