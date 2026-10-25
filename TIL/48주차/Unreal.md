# Replication Graph

Replication Graph는 서버가 클라이언트마다 복제할 Actor를 결정할 때, 매번 모든 Actor를 검사하지 않도록 Actor를 미리 분류하고 필요한 복제 후보를 효율적으로 수집하는 네트워크 복제 시스템이다.

---

## 언제 필요한가?

100명의 플레이어가 있는 오픈월드 서버에 Actor가 20,000개 있다고 가정하자.

- 기존 접근: 플레이어마다 20,000개 Actor를 확인하고 Relevancy 등을 판단한다.
- 문제점: 각 플레이어 주변에는 수백 개만 필요해도 후보를 찾는 검사 자체가 비싸다.
- 개선 목표: 무작정 전부 검사하기 전에 검사할 후보 목록을 좁힌다.

Relavancy는 이 Actor가 이 클라이언트에게 필요한가? 를 판단하는 개념, 반면 Replication Graph는 판단할 Actor후보를 어떻게 효율적으로 모을까? 에 초점을 둔다.

## 어떻게 작동하는가?

대표적인 방법은 공간분할이다.

```
서버의 복제 대상 Actor
        │
        ▼
Replication Graph에서 미리 분류
        │
        ├── GridSpatialization2D (위치 기반)
        │      ├── A구역: 몬스터·상자 목록
        │      ├── B구역: 몬스터·NPC 목록
        │      └── C구역: 몬스터·상자 목록
        │
        └── AlwaysRelevant (위치 무관)
               └── GameState 등

플레이어가 B구역에 있다면
→ B구역 및 필요한 인접 구역의 Actor 목록 수집
→ AlwaysRelevant 목록도 함께 수집
→ 후보에 대해 복제 조건·우선순위·갱신 시점 등을 처리
```

**중요** : Replication Graph가 Actor를 무조건 전송하는것은 아니다. 먼저 효율적으로 후보 목록을 구성하고, 이후 실제 복제 여부·빈도·우선순위 등이 처리된다.

## 플레이어가 이동하면?

플레이어의 새 위치를 기준으로 필요한 격자 목록을 가져온다. 이동하는 Actor 역시 위치에 맞게 공간 관리 정보가 갱신되어야 한다. 그래서 20,000개 전체를 매번 검사하는 대신 주변 Actor를 중심으로 처리할 수 있다.

## 자주 나오는 노드

- UReplicationGraphNode_GridSpatialization2D : 2D 격자 기반으로 Actor를 분류하고 공간별 후보 소집, ex) 주변 몬스터, 상자, NPC
- UReplicationGraphNode_AlwaysRelevant : 위치와 무관하게 필요한 Actor를 전역 목록등으로 관리, ex) GameState등
- UReplicationGraphNode_AlwaysRelevant_ForConnection : 특정 연결에 항상 필요한 Actor 목록, ex) 해당 연결의 PlayerController, ViewTarget
- UReplicationGraphNode_DynamicSpatialFrequency : 이동하는 Actor의 거리 등에따라 복제 빈도 조절을 지원, ex) 동적 Actor의 거리 기반 갱신 정책

> AlwaysRelevant는 복제 후보에서 위치 때문에 제외하지 않는다는 뜻이지, 모든 Actor를 매 프레임 반드시 전송한다는 뜻은 아니다.

## 다른 개념과 구분하기

- Relevancy : 이 Actor가 이 플레이어에게 필요한가?
- NetUpdateFrequency : 복제 갱신을 얼마나 자주 시도하는가?
- Dormancy : 변경이 거의 없는 Actor의 복제를 잠시 쉬게 할 수 있는가?
- Replication Graph : 복제할 Actor 후보 목록을 어떻게 효율적으로 찾고 관리하는가?

## Octree와의 차이

- Octree / Uniform Grid : 주변 객체를 효율적으로 찾는 공간 분할 자료구조
- Replication Graph : Actor를 연결별로 효율적으로 복제하기 위해 여러 규칙과 Actor 목록을 관리하는 시스템
- GridSpatialization2D : Replication Graph 안에서 사용하는 2D 격자 기반 노드, Octree처럼 공간을 재귀적으로 8개로 나누는 방식과는 다르다.

## 주의점

- 격자 크기와 주변 탐색 범위가 너무 크면 후보가 많아지고, 너무 작으면 관리 비용이 커질 수 있다.
- 이동하는 Actor의 위치 갱신, 멀리 떨어진 대형 Actor, 층이 겹치는 3D공간등은 별도로 고려해야한다.
- Replication Graph를 써도 대역폭과 복제 데이터량이 저절로 해결되는 것은 아니다.

# TearOff와 Dormancy의 차이

TearOff는 Actor의 복제를 영구히 종료하는것, 내가 헷갈린것은 초기값만 복제해줌이였는데 이는 Dormancy의 설정이였음

- DORM_Awake : 정상적으로 복제하는 상태
- DORM_DormantAll : 모든 연결에서 휴면 상태, 새 연결에는 초기 상태를 복제할 수 있음
- DORM_Initial : 맵에 미리 배치된 Actor가 처음부터 휴면 상태로 시작하는 설정

# Possess관련 함수 차이

- Possess() : AController, Controller가 Pawn을 조종하도록 설정
- OnPossess() : AController, Controller 입장에서 Possess할 때 실행
- PossessedBy() : APawn, Pawn입장에서 Controller에게 Possess되었을 때 실행

리스폰시 새 캐릭터가 PlayerState에 저장된 정보를 읽어 해당 직업의 능력치를 설정해야 할 때

- 서버에서는 PossessedBy를 통해 Character가 Possess되었을 때 PlayerState에 저장된 직업 정보를 읽어서 Character의 능력치를 초기화한다
- 클라에서는 OnRep_PlayerState로 Character의 PlayerState 참조가 복제되어 갱신되었을 때, 해당 정보를 이용해 필요한 초기화 작업을 수행한다

# RegisterComponent

Component를 생성했을때 Actor에 달아줘야하는건 물론이고, 엔진에 Component를 등록하는 RegisterComponent함수를 써야 TIck이나 물리/렌더링 관련 기능등이 정상적으로 작동함
DestroyComponent로 정리

# 생성자와 BeginPlay차이

## 생성자

- 객체 생성/초기화 과정
- 기본값을 설정하고 컴포넌트 생성하는 목적
- CreateDefaultSubobject로 컴포넌트 생성
- 다른 Actor의 접근은 피하는게 좋음
- GetWorld()사용은 유효한 게임 월드 보장못함
- 실행 횟수는 에디터와 CDO생성 과정에서도 실행될 수 있음

## BeginPlay

- 게임 플레이를 시작할때 실행시점
- 게임 로직 초기화, 이벤트 연결 목적
- 컴포넌트 생성은 일반적으로 사용하지 않음
- 다른 Actor의 접근 가능(유효성 검사 필수)
- GetWorld()의 경우 보통 사용 가능
- 일반적으로 Actor의 플레이 시작 시 한 번 실행

# World Partition

거대한 게임 월드를 여러 구역으로 나누고, 플레이어 주변 구역만 메모리에 로드하고 멀어진 구역은 언로드하는 언리얼 엔진의 월드 관리 시스템
World Partition의 핵심 목적은 네트워크 복제가 아니라 대규모 월드의 로딩과 메모리 관리

## 왜 필요할까?

오픈월드 게임을 개발할때 게임 맵에는 도시, 숲, 사막, 던전 등이 있고, 각 지역에 수많은 건물과 나무, NPC가 배치되어 있다. 플레이어는 현재 도시에서만 활동하고 있다. 이때 사막과 숲에 있는 수많은 Actor까지 전부 로드해 둘 필요는 없다.

플레이어가 오른쪽 지역으로 이동했다고 했을때

1. World Partition이 플레이어의 위치를 기준으로 필요한 셀을 결정함
2. 새로 가까워진 지역의 셀을 로드함
3. 플레이어와 멀어진 지역의 셀은 필요하지 않으면 언로드함
4. 플레이어 주변의 월드만 메모리에 유지할 수 있다.

이때 언로드는 단순히 화면에서 보이지 않게 하는게 아님, 해당 구역의 스트리밍 레벨과 Actor들을 메모리에서 내리는 과정

## Streaming Source

어느 위치를 중심으로 월드를 로드할 것인지 정하는 기준
일반적으로 PlayerController가 Streaming Source 역할을 할 수 있다.
플레이어를 먼 지역으로 순간이동시키고 싶다면

- 목적지에 UWorldPartitionStreamingSourceComponent 를 활성화함
- 목적지 주변 셀이 로딩될 때까지 확인
- 로딩이 완료되면 플레이어를 순간이동시킴

## Is Spatially Loaded

Actor를 위치에 따라 로드할지 결정하는 설정

true : Streaming Source와의 거리 등을 기준으로 로드
false : 거리와 무관하게 로드 대상이 될 수 있음

Data Layer에 할당되어 있다면 해당 레이어의 활성 상태도 영향을 줌

## HLOD(Hierarchical Level of Detail)

멀리 있는 산이나 건물까지 언로드하면, 플레이어 눈에는 아무것도 안 보이는거 아닌가??
이때 HLOD를 활용할 수 있다.
가까운 곳에서는 원래 건물과 나무를 로드하고, 멀리서는 단순화한 메시로 보여주는 방식이야. 렌더링 부담도 줄일 수 있지

## Data Layers

위치뿐 아니라 게임 진행 상황에 따라 특정 콘텐츠를 로드하거나 활성화 하는데 사용하는 기능
예를들어 퀘스트 진행 전에는 마을이 평화롭지만, 퀘스트가 끝나면 마을이 파괴된 상태로 바뀐다고 해보자
Data Layer를 이용하면 상황별 환경 콘텐츠를 구분하고 전환하는 구조를 만들 수 있다.

## 기존 Level Streaming과의 차이

기존 Level Streaming은 개발자가 직접 여러 서브레벨을 만들고 로딩/언로딩을 관리하는 방식이 흔했음
반면 World Partition은 하나의 Persistent Level을 기반으로 공간을 자동 분할하고, Streaming Source를 기준으로 셀을 관리할 수 있다
또한 One File Per Actor(OFPA) 기능과 함께 사용하면 Actor를 개별 파일로 관리할 수 있어서 여러 개발자가 같은 월드를 편집할 때 협업 충돌을 줄이는 데도 도움이됨

# Owner / Instigator / DamageCauser

수류탄이 몬스터에게 피해를 줬다고 생각하면

- Owner : 수류탄의 소유자로 지정된 Actor
- Instigator : 수류탄을 발사하거나 사용하게 만든 Pawn
- DamageCauser : 실제 데미지를 발생시킨 Actor(여기서는 수류탄)

# Idempotency

멱등성, 같은 요청이 여러번 들어와도 결과는 한 번 처리한것과 같아야 한다는 뜻
만약에 서버에서 보스킬에 대한 보상을 준다고 하면 이를 RewardId로 관리하고 같은 클라에서 요청을 여러번 보내도 하나의 요청만 들어주게끔
그리고 만약 서버에서 골드를 지급하고 완료되기전에 네트워크 연결이 끊겼을때는 골드 지급과 지급 완료 기록을 하나의 원자적인 작업으로 처리하면 됨
만약에 골드 지급 후 처리 결과를 저장했는데 성공 응답이 클라에 전송되지 않은 상태에서 끊겼다면 재접속 후 같은 보상을 요청할때 서버가 저장된 지급기록을 확인하고, 이미 지급한 보상이면 추가 지급하지 않음, 클라이언트에 기존 지급 완료 상태를 알려준다.

# BlueprintPure, BluepintCallable의 차이

# 충돌 이벤트

- QueryOnly가 뭐였는지
- ECR, ECC차이

# FName, FText, FString차이

# HUD 함수들

- CreateWidget : 위젯 객체 생성
- SetVisibility : 위젯의 표시 상태 설정
- AddToViewport : 실제 게임 화면에 위젯 추가
