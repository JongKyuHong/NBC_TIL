# Unreal Engine, UCLASS와 게임의 구성

## 1) UCLASS란?

- 정의
- UCLASS()는 언리얼의 리플렉션(Reflection) 시스템에 클래스를 등록하는 매크로입니다. 리플렉션에 등록된 클래스는 엔진이 다음 정보를 다룰 수 있다.
- 타입 정보(Type info)
- 프로퍼티/함수 메타데이터
- 직렬화(저장/로드)
- 에디터 노출(Details 패널, BP 편집 등)
- GC(가비지 컬렉션) 추적
- 네트워크 복제(Replication) 관련 메타데이터

### 왜 필요한가?

- C++만으로는 `이 변수는 저장해야 하는지`, `에디터에 보여줘야 하는지`, `블루프린트에서 호출 가능한지`, `복제해야 하는지` 같은 정보를 엔진이 일관되게 알기 어렵다.
- UCLASS / UPROPERTY / UFUNCTION로 `엔진이 알아야 하는 정보`를 명시하면, 엔진이 자동으로 처리할 수 있게 된다.

### 핵심 포인트

- UCLASS()가 붙는 클래스는 보통 UObject 계열 (예: AActor, UActorComponent, UUserWidget)
- 에디터 노출/저장/복제가 필요한 멤버는 일반 C++ 멤버가 아니라 UPROPERTY()로 선언하는 것이 기본 전제
- 블루프린트에서 호출되거나 이벤트로 바인딩될 함수는 **UFUNCTION()**으로 메타정보를 붙입니다.

## 2. GameModeBase: 서버 권한의 규칙(룰)

- 정의/역할
- AGameModeBase는 게임 규칙(룰)의 **권위(authority)**를 갖는 클래스
- 스폰 규칙, 리스폰 규칙
- 승패 조건
- 게임 시작/종료 흐름
- 접속/퇴장 처리
- 팀 배정, 라운드 진행 등 `서버만 판단하면 되는 로직`

### 네트워크에서의 존재 위치

- 서버에만 존재
- 그래서 클라이언트에서 GetGameMode()가 null인 것은 정상

### 실무 포인트

- 모든 클라이언트가 알아야 하는 상태(남은 시간, 점수, 라운드 상태 등)를 GameMode에 두면 안됨!!
- 이런 값은 GameState로 복제시키는 구조가 기본이다.
- 또한 DefaultPawnClass, PlayerControllerClass, HUDClass, GameStateClass 같은 `게임 구성 요소`도 보통 GameMode에서 지정

## 3. GameInstance: 프로세스 단위 전역 컨텍스트

### 정의

- UGameInstance는 게임 프로세스(실행)당 1개 존재하는 전역 컨텍스트
- 레벨 전환(OpenLevel, Seamless Travel)에도 유지되며, 게임 종료 시 함께 종료
- 프로젝트 설정에서 GameInstance 클래스를 지정할 수 있다.

### 생명주기

- 생성: 게임 실행(또는 PIE 시작) 시 생성
- 유지: 레벨 로딩/언로딩과 무관하게 유지
- 종료: 게임 종료(또는 PIE 종료)

### 대표 훅(흐름 파악용)

- Init() : 초기화 진입점
- OnStart() : `게임이 시작 상태`로 들어갈 때 호출되는 성격
- Shutdown() : 종료 정리

### 네트워크 관점(중요)

- GameInstance는 서버/클라이언트 각각 자기 프로세스에만 존재 / 즉, 복제(Replication) 대상이 아니다.
- 따라서 “모든 플레이어가 공유해야 하는 경기 상태”를 GameInstance에 두면 안 됨
- 공유/동기화는 GameState / PlayerState / Replicated Actor로 처리

- GameInstance = 로컬/프로세스 단위 전역 상태
- GameState/PlayerState = 네트워크로 공유되는 상태

### 언제 쓰나(현업에서 흔한 사용처)

- 레벨을 넘어 유지되어야 하는 데이터: 로그인/계정(로컬), 사용자 옵션, UI 플로우 상태
- 온라인 세션/매치메이킹 정보(온라인 서브시스템 연동)
- 로딩 화면/전역 UI 매니저 같은 `전역 서비스`

### 권장 구조: GameInstanceSubsystem

- GameInstance가 비대해지지 않게, 전역 서비스는 보통 Subsystem으로 분리
- UGameInstanceSubsystem: GameInstance에 붙는 전역 서비스 모듈
- 예: Save/Load, 세션, 통계, 로깅, 데이터 로딩 등

### 주의점

- (실수1) 점수/라운드/남은 시간 같은 “공유 상태”를 GameInstance에 저장 → GameState로 보내야 함
- (실수2) 레벨 전환으로 파괴되는 Actor/Component 포인터를 GameInstance가 붙잡고 있음 → 댕글링 참조 위험
  해결 원칙:
- 레벨 종속 객체 참조는 TWeakObjectPtr로 들고 IsValid() 체크
- 에셋 참조는 TSoftObjectPtr / TSoftClassPtr로 들고 필요 시 로드
- 반드시 유지되어야 하는 UObject면 소유/수명 구조를 명확히(Subsystem + UPROPERTY 패턴)

- (실수3) PIE에서 인스턴스가 여러 개처럼 보임: 멀티 PIE(서버/클라)에서 프로세스/인스턴스가 분리되어 생기는 현상

### 접근 방법

- C++: 월드가 있으면 GetGameInstance()
- 블루프린트: Get Game Instance 노드

## 4. GameState: 매치의 “공유 상태” 컨테이너

### 정의/역할

- GameMode가 규칙을 집행한다면, AGameStateBase는 그 결과로 나온 “현재 상태”를 모든 클라이언트에 전파

### 네트워크에서의 존재 위치

- 서버에 존재 + 클라이언트로 복제(Replicate) 됨
- 클라이언트는 GetGameState()로 신뢰 가능한 현재 경기 정보를 받음

### 언제 쓰나

- 남은 시간, 라운드 번호, 목표 진행률
- 팀 점수, 점령 상태, 경기 페이즈(대기/진행/종료)

### 실무 포인트

- UI에 보여야 하는 경기 전체 정보는 GameState에 두면 구조가 매우 깔끔
- 일반적으로 복제 변수 + OnRep 콜백 중심으로 설계

## 5. PlayerState: 플레이어 1명의 “공유 상태”

### 정의/역할

- APlayerState는 “플레이어 한 명”의 공유 상태를 담음
- 킬/데스/점수
- 닉네임, 팀, 핑
- 스코어보드에 표시되는 정보
- Pawn이 교체되어도 유지되어야 하는 정보

### 네트워크에서의 존재 위치

- 서버에 존재 + 모든 클라이언트에 복제됩니다.
- 즉, 한 플레이어의 PlayerState는 다른 플레이어 클라이언트에서도 볼 수 있는 것이 일반적입니다.

### 실무 포인트

- `플레이어 본인만 알아야 하는 비공개 정보`를 PlayerState에 넣으면 곤란하다. (복제되기 쉬움)
- `플레이어 장기 상태`는 PlayerState, `현재 조종 중 캐릭터 상태(체력/버프)`는 Pawn/Character로 분리하면 안전

## 6. HUD: 전통적 UI 진입점 (요즘은 UMG 위주)

### 정의/역할

- AHUD는 전통적인 HUD 기반 UI 진입점으로, 특히 DrawHUD()에서 Canvas 기반 렌더링을 담당합니다.

### 네트워크에서의 존재 위치

- 일반적으로 로컬(해당 플레이어 클라이언트)에만 존재
- 서버/다른 클라이언트에 내 HUD가 복제되는 개념은 아님

### 실무 포인트

- 요즘은 UMG(UUserWidget)를 주로 쓰고
- HUD는 위젯 생성/관리를 위한 얇은 래퍼로만 쓰거나 아예 생략하는 경우도 많다.
- 입력/상호작용 흐름은 보통 PlayerController ↔ Widget 연결이 중심

## 7. PlayerController: 입력과 소유(Ownership)의 중심

### 정의/역할

- APlayerController는 플레이어 입력 처리와 소유 관점의 중심
- 플레이어(사람)를 대표하며 Pawn을 Possess한다.

### 네트워크에서의 존재 위치

- 서버에 존재
- **소유한 클라이언트(owning client)**에도 존재
- 다른 플레이어의 PlayerController는 내 클라이언트에 보통 존재하지 않거나 접근 대상이 아니다.

### 언제 쓰나

- 입력 바인딩, 카메라/조작 흐름
- Pawn 교체/리스폰 시 Possession 처리
- UI 생성/토글, 커서/입력 모드 설정
- `플레이어 개인 로컬 로직` 배치

### 실무 포인트

- Controller는 `플레이어의 껍데기`
- Pawn은 `조종되는 몸체`
- 리스폰/캐릭터 변경 게임에서는 Controller는 유지되고 Pawn만 교체되는 패턴이 흔함

### 8. Pawn / Character: 조종되는 몸체

### Pawn

- Pawn은 플레이어/AI가 조종 가능한 액터의 기본 단위입니다. Controller에 의해 Possess 될 수 있다.

### 네트워크에서의 존재 위치

- 서버에 존재하고 필요 시 클라이언트에 복제
- `내가 조종하는 Pawn`은 로컬에도 존재하며, 네트워크 규칙에 따라 움직임/상태가 동기화

### Character

- ACharacter는 APawn을 상속한 사람형 캐릭터 표준 구현 (캡슐, 메시, CharacterMovement 포함)

### 실무 포인트

- 장기 정보: PlayerState
- 현재 몸체 상태(체력/상태 이상): Pawn/Character
- 입력은 Controller에서 받고, 실제 이동/행동은 Pawn에서 처리하는 구조가 흔함

## 9. Actor / Component: 월드 오브젝트와 기능 분해

### Actor

- 월드에 배치되는 대부분의 게임 오브젝트의 기본 단위
- Transform, 컴포넌트 구성
- Tick
- Replication
- 충돌/물리

### Component

- Actor에 기능을 부품처럼 붙이는 단위
- UActorComponent: 로직 중심(Transform 없음)
- USceneComponent: Transform 있음(Attach 가능)
- UPrimitiveComponent: 렌더/충돌/물리 포함(StaticMeshComponent 등)

### 실무 포인트

- Actor는 컨테이너이고, 실제 기능은 컴포넌트로 분해하는 설계가 많음
- 또한 성능/구조 관점에서 Tick 남발이 흔한 문제이므로 필요할 때만 Tick을 켜는 습관이 중요하다!!!

## 10. BeginPlay / EndPlay: 수명 훅

### BeginPlay

- Actor/Component가 “실제 게임 월드에서 플레이를 시작”할 때 1회 호출됨
- 월드 접근, 다른 Actor 찾기, 타이머/입력 연결 등은 보통 BeginPlay에서 한다.
- 생성자에서는 월드가 없거나(에디터/프리뷰), 아직 준비가 안 된 경우가 많음

### EndPlay

- Actor가 월드에서 제거되거나 플레이가 끝날 때 호출됨
- 타이머 해제, 델리게이트 언바인딩, 외부 참조 정리 같은 `정리 작업`은 EndPlay가 안전!!!

## 11. 생성자/소멸자와 CDO (매우 중요)

### 생성자(Constructor)에서 일어나는 일

- UObject 계열은 엔진이 `클래스 기본값`과 `인스턴스 초기화`를 위해 생성자를 다양한 상황에서 호출함

### 보통 생성자에는 다음이 들어감

- CreateDefaultSubobject로 기본 컴포넌트 구성
- 디폴트 값 설정

### 생성자에서 피해야 하는 것

- 월드 의존 로직(다른 Actor 찾기, 타이머 시작, GameState 접근 등)
- 런타임 데이터 기반 초기화(세이브 로드 등)
- 이런 것들은 BeginPlay 또는 PostInitializeComponents 등으로 미루는 것이 안전함

### 소멸자(Destructor)를 조심해야 하는 이유

- 언리얼의 UObject 수명은 GC 기반이며, 일반 C++처럼 “스코프 끝나면 즉시 소멸” 모델이 아님
- 그래서 정리는 보통 다음 훅을 우선시함
  - EndPlay
  - BeginDestroy, ConditionalBeginDestroy
  - OnComponentDestroyed, OnDestroyed

### CDO(Class Default Object)

- 각 UCLASS마다 1개 존재하는 `클래스의 기본 인스턴스`
- 엔진은 CDO를 통해 기본 값을 저장하고, 이후 인스턴스는 CDO를 기준으로 초기화됨

- _중요: 생성자는 CDO 생성에도 호출_
- 따라서 생성자에 런타임 전제(월드/플레이어 존재)를 넣으면 CDO 단계에서부터 논리 충돌이 날 수 있다.

# 코드카타

## 두 원 사이의 정수 쌍

```c++
#include <string>
#include <vector>
#include <cmath>

using namespace std;

long long solution(int r1, int r2) {
    long long answer = 0;

    // r1의 반지름값보다 같거나 크고, r2의 반지름보다 작거나 같은 정수쌍 나열임
    // y값이 1부터 시작하고 최대 r2의 반지름 만큼 확인?

    long long cnt = 0;

    for (int y = 1; y <= r2; y++){
        for (int x = 0; x <= r2; x++){
            if (pow(r1, 2) <= pow(y, 2) + pow(x, 2) && pow(r2 ,2) >= pow(y, 2) + pow(x, 2)){
                cnt++;
            }
        }
    }

    return cnt*4;
}

```

- 처음에는 완전탐색을 돌렸는데 당연하게도 입력이 100만이라 시간초과가 났다./

```c++
#include <string>
#include <vector>
#include <cmath>
#include <iostream>

using namespace std;

long long solution(int r1, int r2) {
    long long answer = 0;

    long long cnt = 0;

    for (int x = 1; x <= r2; x++){
        // 원의 경계 안쪽으로 들어와야 해서 floor
        long long maxY = floor(sqrt((long long)r2 * r2 - (long long)x * x));

        // r1원의 경계 밖으로 나와야 해서 ceil
        long long minY = 0;
        if (x < r1){
            minY = ceil(sqrt((long long)r1 * r1 - (long long)x * x));
        }

        answer += maxY - minY + 1;
    }
    return answer*4;
}
```

- x를 1에서부터 r2반지름까지 확인하면서 최대y, 최소y를 구하고 +1 (최소가 2, 최대가 4일때 2, 3, 4를 세야하므로)
- 그리고 \*4 (이거 해주기 위해서 x를 1부터봤다)
