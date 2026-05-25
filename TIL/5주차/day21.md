![](https://velog.velcdn.com/images/kyu_/post/b3b671de-2bc3-42a9-bdeb-53d05286a996/image.png)

## 5번 과제하기

### 1. 시작 위치 설정하기

```c++
FVector StartLocation(0.0f, 50.0f, 0.0f);
SetActorLocation(StartLocation);
```

- 벡터이므로 x, y, z를 뜻한다.
- f는 float 리터럴을 나타내는 C++의 접미사이다.
- C++에서 소수점 리터럴은 기본적으로 double타입이고 f를 붙이면 float로 변환
- FVector의 경우 float를 사용하기 때문에 타입 불일치가 생길 수 있다. 그래서 f를붙여줌
- StartLocation은 임의로 변수이름을 지은것이고 SetActorLocation()은 액터의 월드 좌표를 설정한다.

### 2. 로그 출력하기

```c++
GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Blue, FString::Printf(TEXT("위치 출력 >>> X : %f, Y : %f, Z : %f"), StartLocation.X, StartLocation.Y, StartLocation.Z));
```

- AddOnScreenDebugMessage를 통해서 인게임내 로그출력을 한다.
- 첫번째 인자 : 메시지 식별 키
  - -1일시 : 매번 새로운 줄에 메시지 추가
  - 양수일시 : 같은 Key의 이전 메시지를 덮어씌운다.
- 두번째 인자 : 표시 시간
- 세번째 인자 : 색상
- 네번째 인자 : 디버그 메시지(출력 메시지)
- 다섯번째 인자 : 최신 메시지 위치(기본값 true : 새 메시지가 위쪽에 표시)
- 여섯번째 인자 : 텍스트 크기
- FString과 Printf를 통해서 TEXT에 변수를 찍어 줄 수 있다.

### 3. Turn, Move함수 만들기

- MyActor.h에서 MyActor클래스의 private에 Turn, Move, EventPrint함수를 선언해 놓는다.
- 총 이동거리, 이벤트 발생 횟수, Move함수 호출 수, Turn함수 호출 수, 이동 종료 변수도 선언

```c++
class ASSIGNMENT5_API AMyActor : public AActor
{
...
private:
	void Move();
	void Turn();
    void EventPrint();

    int MoveCount;
    int TurnCount;
    int EventCount;
    float TotalDistance;
    bool bCompleteEvent;
};
```

#### Move함수 만들기

```c++
void AMyActor::Move() {
	// 현재 바라보는 방향의 벡터
	FVector ForwardVector = GetActorForwardVector();

	// 랜덤 이동값
	float MoveDistance = FMath::FRandRange(500.0f, 1000.0f);

	// 새 위치 계산
	FVector PrevLocation = GetActorLocation();
	FVector NewLocation = PrevLocation + (ForwardVector * MoveDistance);

	// Z축 고정으로 평면 이동만
	NewLocation.Z = PrevLocation.Z;
	SetActorLocation(NewLocation);

	// 이동거리 계산
	float dx = NewLocation.X - PrevLocation.X;
	float dy = NewLocation.Y - PrevLocation.Y;
	float Distance = FMath::Sqrt(dx*dx + dy*dy);

	// 총 이동거리 누적
	TotalDistance += Distance;

	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Orange,
		FString::Printf(TEXT("%d번 이동, 현재위치 >>> X : %f, Y : %f, Z : %f"),
			MoveCount+1, NewLocation.X, NewLocation.Y, NewLocation.Z));
}
```

- GetActorForwardVector를 구하고, 이를 랜덤 이동값과 곱하고 기존 액터 위치와 더한값을 새로운 위치로 저장한다.
- Z축은 고정하고 평면에서만 위치가 변할 수 있게 Z값은 계속 유지하려고 했다.
- 로그를 찍을때 헤더에서 private으로 설정했던 MoveCount를 찍어주고 새로운 좌표의 X,Y,Z를 출력했다.

#### Turn함수 만들기

```c++
void AMyActor::Turn() {
	// 랜덤 회전 값
	float RandomRotateValue = FMath::FRandRange(-180.0f, 180.0f);

	FRotator NewRotation = GetActorRotation();

	// 좌우회전에 랜덤값 추가
	NewRotation.Yaw += RandomRotateValue;

	SetActorRotation(NewRotation);

	GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Magenta, FString::Printf(TEXT("%d번 회전 >> %f도"), TurnCount+1, RandomRotateValue));
}
```

- GetActorRotation으로 액터의 회전 값을 구하고 랜덤 값을 Yaw에 더해주고 저장

### 4. Tick() 수정하기

- Tick()

```c++
// Called every frame
void AMyActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// 이미 10회 이동하면 더이상 이동 안함
	if (bCompleteEvent) {
		return;
	}

	static float TimeAccumulator = 0.0f;
	TimeAccumulator += DeltaTime;

	// 0.5초마다 실행
    if (TimeAccumulator >= 0.5f) {
		TimeAccumulator = 0.0f;
		if (MoveCount + TurnCount < 10) {
			// 50퍼센트 확률로 이동 or 회전 선택
			if (FMath::RandRange(0, 1)) {
				Move();
				MoveCount++;
				// 50퍼센트 확률로 이벤트 발생
				if (FMath::RandRange(0, 1)) {
					EventPrint(TEXT("이동"));
					EventCount++;
				}
			} else {
				Turn();
				TurnCount++;
				if (FMath::RandRange(0, 1)) {
					EventPrint(TEXT("회전"));
					EventCount++;
				}
			}
		} else {
			// 10회 완료
			bCompleteEvent = true;

			// 통계
			GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Cyan, FString::Printf(TEXT("이동끝")));
			GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Cyan, FString::Printf(TEXT("총 이동거리 : %f"), TotalDistance));
			GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Cyan, FString::Printf(TEXT("총 이벤트 발생 횟수 : %d"), EventCount));
		}
	}
}
```

- DeltaTime은 이전 프레임과 현재 프레임 사이의 시간 차
  - 60FPS환경에서는 1/60초 즉 0.0166초
  - Accumulator를 0.5f로 지정해서 0.5초마다 이동이 가능하게 하였다.
- 이동시 50퍼센트 확률로 Move() 혹은 Turn()이 호출된다.
  - 각각 MoveCount, TurnCount로 카운팅된다.
- 또, 이동시 50퍼센트 확률로 랜덤 이벤트가 발생할 수 있다. (메시지만 출력)
- MoveCount+TurnCount가 10이되면 이동을 멈추고 통계를 로그로 띄우고 끝나게 된다.

### EventPrint() 함수 수정

```c++
void AMyActor::EventPrint(const FString& EventType) {
	GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Red,
		FString::Printf(TEXT("% s 시 랜덤 이벤트 발생!!!!!!!"), *EventType));
}
```

- Move()냐 Turn()이냐에 따라 다르게 로그를 출력시켜주었다.

## 코드카타

### set을 vector로 변환

- 두개 뽑아서 더하기 문제

```c++
#include <string>
#include <vector>
#include <unordered_set>
#include <algorithm>

using namespace std;

vector<int> solution(vector<int> numbers) {
    unordered_set<int> us;
    for (int i = 0; i < numbers.size()-1; i++){
        for (int j = i+1; j < numbers.size(); j++){
            us.insert(numbers[i]+numbers[j]);
        }
    }

    vector<int> answer(us.begin(), us.end());

    sort(answer.begin(), answer.end());

    return answer;
}
```

- vector를 선언할때 셋의 이터레이터를 넘겨주어 셋을 안정적으로 벡터로 변환할 수 있다.

### 람다 함수 정렬에서 외부변수 사용하기

- 문자열내마음대로정렬하기

```c++
#include <string>
#include <vector>
#include <algorithm>

using namespace std;

vector<string> solution(vector<string> strings, int n) {
    vector<string> answer;

    sort(strings.begin(), strings.end(), [n](const auto& a, const auto& b){
        if (a[n] == b[n]){
            return a < b;
        }
        return a[n] < b[n];
    });

    return strings;
}
```

## 언리얼 게임플레이 프레임워크 클래스 구조

| 클래스           | 역할                       | 생명주기            | 네트워크          |
| ---------------- | -------------------------- | ------------------- | ----------------- |
| GameInstance     | 전역 관리자, 세이브/로드   | 게임 시작~종료      | 복제 안 됨        |
| GameMode         | 게임 규칙, 스폰 로직       | 레벨 로드~언로드    | 서버만 존재       |
| GameState        | 공유 상태 (점수, 타이머)   | 레벨 로드~언로드    | 복제됨            |
| PlayerState      | 플레이어별 상태 (HP, 킬수) | 플레이어 입장~퇴장  | 복제됨            |
| PlayerController | 입력, UI, 카메라           | 플레이어 생성 시    | 소유 클라이언트만 |
| Character        | 캐릭터 물리/시각           | 스폰~파괴           | 복제됨            |
| ActorComponent   | 재사용 가능한 기능         | 소유 Actor 생명주기 | 선택적            |

## C++ vs Blueprint분리

### C++

- 게임 로직 핵심
- 재사용 가능 컴포넌트
- 성능이 중요한 부분(충돌 처리, AI)
- 인터페이스

### 블루프린트

- 데이터 중심 클래스 (무기 종류, 적 타입)
- 빠른 프로토타이핑
- 레벨 스크립팅
- UI로직

## 언리얼의 설계 철학

- 게임 플레이 프레임워크 + 컴포넌트 패턴 + 이벤트 시스템의 조합

### 1. 책임 분리 체계(계층 구조)

- 언리얼은 게임을 만들때 각 요소가 명확한 역할을 갖도록 설계되어 있다.
- GameMode : 이 게임은 어떻게 진행되는가?
- GameState : 모든 플레이어가 공유해야하는 데이터 저장
- PlayerController : 플레이어의 입력과 UI
- PlayerState : 플레이어별 정보
- Pawn/Character : 물리적 표현, 게임세계에서 보이는 것

### 2. 컴포넌트 패턴(기능 분리)

- 가장 중요한 설계 원칙
- 기능별로 컴포넌트를 만들어서 조립한다.

### 3. 이벤트 시스템(느슨한 결합)

- 각 시스템이 서로를 직접 참조하지 않고 "이벤트"를 통해 통신한다.

### 4. 데이터 주도 설계

- 코드에 숫자를 직접 쓰지않고 "데이터 에셋"에 저장한다.
- 기획자가 밸런스 조정 가능
- A/B테스트 쉬움

### 5. 인터페이스 패턴(다형성)

- 상호작용 가능한 모든것을 하나의 방식으로 다룬다.
- 예를들어 문열기, 아이템줍기, npc대화 각각 다른 클래스인데 어떻게 E키=상호작용 하나로 처리??
- 인터페이스로 해결
