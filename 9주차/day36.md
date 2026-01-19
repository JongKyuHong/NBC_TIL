# 7번 과제

- [x] C++ Pawn클래스만들기
  - [x] CapsuleComponent or BoxComponent or SphereComponent 만들기
  - [x] SkeletalMeshComponent 만들기
  - [x] SpringArmComponent 만들기
  - [x] CameraComponent 만들기
  - [x] 충돌 컴포넌트를 루트로 설정하고 부착하기
  - [x] DefaultPawn 설정하기
- [x] Physics설정하기
  - [x] 루트 충돌 컴포넌트, SkeletalMeshComponent모두 Simulate Physics = false로 설정하고 코드로 제어해보기

- [x] Enhanced Input 액션 설정
  - [x] Move
  - [x] Look
  - [x] IMC에서 매핑
  - [x] SetupPlayerInputComponent()에서 바인딩하기

- [x] Pawn 이동 로직 작성하기
  - [x] AddActorLocalOffset, AddActorLocalRotation등 입력에 따라 Pawn 움직이기
    - [x] 이동 방향은 Pawn의 Forward/Right Vector로 결정
    - [x] 마우스 입력으로 Yaw와 Pitch 계산하여 회전 구현하기
      - [x] AddControllerYawInput, AddControllerPitchInput 같은 기본제공함수 사용하지 않기
  - [x] DeltaTime 사용해 프레임 독립적인 이동 구현하기

--- 도전과제 ---

- [ ] 6자유도 드론 구현하기
  - [ ] 전후좌우상하 구현
  - [ ] Yaw, Pitch, Roll 회전 구현하기
  - [ ] Local 기반 이동 구현하기
    - [ ] 현재 Pawn의 회전상태에 따라 이동방향이 결정되는 비행체 움직임 구현
    - [ ] 단순 월드 좌표계 이동이 아닌 Pawn의 로컬 좌표계 기준으로 이동 구현

- [ ] 중력 및 낙하 구현
  - [ ] 인공 중력 구현
    - [ ] Tick함수로 매 프레임 중력 가속도를 계산, 중력상수를 사용해 낙하속도 구현
    - [ ] LineTrace, SweepTrace를 사용해 지면 충돌 감지
    - [ ] 착지 시 Z축 속도 0으로 초기화
  - [ ] 에어 컨트롤 구현
    - [ ] 공중에서는 지상속도의 30~50%로 설정하기
    - [ ] 지상/공중 상태에 따라 이동 로직을 구분하여 자연스러운 움직임 구현하기

## Pawn 클래스 만들기

- [참고](https://rhksgml78.tistory.com/698)
- [참고2](https://roanne.tistory.com/34)
- [참고3](https://vetenir.tistory.com/34)

```
// MyPawn.h
protected:
	virtual void BeginPlay() override;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	UCapsuleComponent* RootComp;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	USkeletalMeshComponent* SkeletalMeshComp;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera")
	USpringArmComponent* SpringArmComp;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera")
	UCameraComponent* CameraComp;

// MyPawn.cpp
AMyPawn::AMyPawn()
{
	RootComp = CreateDefaultSubobject<UCapsuleComponent>(TEXT("RootCapsule"));
	SetRootComponent(RootComp);

	SkeletalMeshComp = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletalMesh"));
	SkeletalMeshComp->SetupAttachment(RootComp);

	static ConstructorHelpers::FObjectFinder<USkeletalMesh> MeshAsset(TEXT("/Game/Resources/Characters/Meshes/SKM_Manny.SKM_Manny"));
	if (MeshAsset.Succeeded())
	{
		SkeletalMeshComp->SetSkeletalMesh(MeshAsset.Object);
	}

	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArmComp->SetupAttachment(RootComp);
	SpringArmComp->TargetArmLength = 400.0f;
	SpringArmComp->SetRelativeRotation(FRotator(-45.0f, 0.0f, 0.0f));
	SpringArmComp->bUsePawnControlRotation = true;

	CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	CameraComp->SetupAttachment(SpringArmComp);
	CameraComp->bUsePawnControlRotation = false;
}
```

- PawnClass에 UCapsuleComponent, USkeletalMeshComponent, USpringArmComponent, UCameraComponent 추가
- 블루프린트에서 수정가능하게 리플렉션 시스템에 등록

## 이동 구현하기

### 대각 이동시 가속

```c++
void AMyPawn::Move(const FInputActionValue& Value)
{
	FVector2D MovementVector = Value.Get<FVector2D>();
	float DeltaTime = GetWorld()->GetDeltaSeconds();

	if (!MovementVector.IsNearlyZero())
	{
    // 벡터 정규화로 크기를 1로 만든다.
    MovementVector.Normalize();
		FVector DeltaLocation(MovementVector.Y * MoveSpeed * DeltaTime, MovementVector.X * MoveSpeed * DeltaTime, 0.0f);
		AddActorLocalOffset(DeltaLocation, true);
	}
}
```

- 기존 Move에서는 이렇게 코드를 사용하고 있었다.
  - WA를 같이누르면 1의 속도가 아닌 루트2의 속도로 빠르게 이동하는 버그가 있었다.
  - MovementVector.Normalize()를 추가해서 정상적으로 변경되었다.

```
정규화 전:
W: (0, 1) → 크기 1
WA: (-1, 1) → 크기 1.414

정규화 후:
W: (0, 1) → 크기 1
WA: (-0.707, 0.707) → 크기 1
```

## 공부한 내용

### 입력

```c++
if (AMyPlayerController* PC = Cast<AMyPlayerController>(Controller))
{
	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PC->GetLocalPlayer()))
	{
		Subsystem->AddMappingContext(DefaultMappingContext, 0);
	}
}
```

- 향상된 입력 시스템은 '서브시스템'이라는 매니저가 관리한다.
- 플레이어의 로컬 환경에서 이 매니저를 찾아야만 키 매핑을 추가하거나 제거할 수 있다.
- 실제로 매핑 컨텍스트를 추가하고 0(우선순위를 부여한다)

### 이동

```c++
void AMyPawn::Move(const FInputActionValue& Value)
{
	FVector2D MovementVector = Value.Get<FVector2D>();
	float DeltaTime = GetWorld()->GetDeltaSeconds();

	if (!MovementVector.IsNearlyZero())
	{
		// 벡터 정규화로 크기를 1로 만든다.
		MovementVector.Normalize();
		FVector DeltaLocation(MovementVector.Y * MoveSpeed * DeltaTime, MovementVector.X * MoveSpeed * DeltaTime, 0.0f);
		AddActorLocalOffset(DeltaLocation, true);
	}
}
```

-FVector DeltaLocation(MovementVector.Y _ MoveSpeed _ DeltaTime, MovementVector.X _ MoveSpeed _ DeltaTime, 0.0f);

- FVector의 X에 MovementVector의 Y값을 넣고, Y에 MovementVector의 X값을 넣었다.
  - 언리얼 좌표계 (X: Forward, Y: Right)가 이렇게 되어있어서 그렇다.
  - IA와 혼용되어서 여기서 시간이 가장 많이 들었다.
- AddActorLocalOffset의 두번째 인자는 bSweep
  - 쓸어넘기듯 검사하며 이동한다. 즉, 경로에 충돌체가 있으면 거기서 이동을 멈춘다.
  - false로 하면 통과해서 넘어가버림
- 좌표 직접 제어(Offset) 방식
  - Local Offset: 캐릭터가 바라보는 방향을 기준으로 이동 (예: 캐릭터가 90도 회전해 있으면, 캐릭터 기준 앞쪽으로 이동)
  - World Offset: 캐릭터의 방향과 상관없이 세상의 절대적인 X, Y, Z축을 기준으로 이동

#### 입력의 X, Y vs 월드의 X, Y

해당 부분 이해에 가장 많은 시간이 들었다...
아래는 제미나이가 정리해준 차이점

- 입력(IA_Move)의 기준:
  - X축 (가로): 왼쪽(-1.0) / 오른쪽(+1.0) → 보통 A, D 키
  - Y축 (세로): 아래쪽(-1.0) / 위쪽(+1.0) → 보통 S, W 키

- 언리얼 월드(World)의 기준:
  - X축 (Forward): 앞(+), 뒤(-)
  - Y축 (Right): 오른쪽(+), 왼쪽(-)
  - Z축 (Up): 위(+), 아래(-)

**입력의 Y(앞/뒤 성분)**를 **월드의 X(전진 방향)**에 대입

#### 언리얼의 왼손 좌표계

- 엄지손가락을 하늘로 향하게 합니다 (Z축: Up).
- **집게손가락(검지)**을 앞방향으로 찌릅니다 (X축: Forward).
- **가운데손가락(중지)**을 오른쪽으로 꺾습니다 (Y축: Right).

### 회전

```c++
void AMyPawn::Look(const FInputActionValue& Value)
{
	FVector2D LookAxisVector = Value.Get<FVector2D>();
	float DeltaTime = GetWorld()->GetDeltaSeconds();

	float YawInput = LookAxisVector.X * RotationSpeed * DeltaTime;
	AddActorLocalRotation(FRotator(0.0f, YawInput, 0.0f));

	if (SpringArmComp)
	{
		float PitchInput = -LookAxisVector.Y * RotationSpeed * DeltaTime;
		FRotator NewSpringArmRotation = SpringArmComp->GetRelativeRotation();
		NewSpringArmRotation.Pitch = FMath::ClampAngle(NewSpringArmRotation.Pitch + PitchInput, -70.0f, 20.0f);
		SpringArmComp->SetRelativeRotation(NewSpringArmRotation);
	}
}
```

- Yaw(좌우회전) : 캐릭터 몸 회전시키기
  - Z축을 기준으로 회전(왼손법칙에서 엄지손가락을 축으로 돌아가는 방향)
  - AddActorLocalRotation으로 회전

- Pitch(상하회전) : 카메라 회전
  - Pitch(왼손 법칙에서 중지를 축으로 돌아가는 방향)
  - SpringArm을 돌린다
  - Y축 반전을 위해 PitchInput에 넣을때 -를 붙여줌
  - ClampAngle는 각도를 특정 범위내에서 고정(Clamp)
    - 첫번째인자 : 입력
    - 두번째 인자 : 최소값
    - 세번째 인자 : 최대값

![](https://velog.velcdn.com/images/kyu_/post/a1bd8efc-f023-4a31-8379-e6dfbb7b0c4e/image.png)

# 챌린지 반

- 철권이라고 했을때 캐릭터의 상태가 뭐가있나??
- Idle, Attack, Guard, Moving, Jumping, Landing
- 캐릭터의 상태를 나타내려면
  private:
  bool bIsJumping;
  bool bIsAttacking;
  bool bIsBlocking;

이런식으로 모든 상태를 다 써주어야 한다. 이런경우 변수이름 변경도 어렵고 노가다방식

- 이를 해결하기위해 게임플레이 태그가 생김
- GameplayTags = 계층적 태그 시스템
- 점으로 구분된 계층적 문자열 태그 시스템
- Character.State.Jumping
- FName기반으로 사용 : 이름 맞추는게 빠르다, 내부적으로 int를 사용, 인덱싱이 빠르다, 에디터에서 관리가 가능하다.

## 참가자의 상태를 태그로 설계 해보자

- 내 답

```
User.States.Speaking
User.States.Mute
User.States.Presentation
User.States.OnCamera
User.States.OffCamera
```

- 실제 답

```
Meeting.State.Speaking
Meeting.Audio.Muted
Player.Network.Unstable
Player.Audio.Muted
Meeting.Network.Unstable
Player.Role.Host
Player.Video.On
Player.Video.Off
```

# 코드카타

## 할인행사

```c++
#include <string>
#include <vector>
#include <unordered_map>
#include <iostream>

using namespace std;

int solution(vector<string> want, vector<int> number, vector<string> discount) {
    int answer = 0;
    unordered_map<string, int> counts;
    unordered_map<string, int> goal;


    for (int i = 0; i < want.size(); i++){
        goal[want[i]] = number[i];
    }

    for (int i = 0; i < 10; i++){
        counts[discount[i]]++;
    }

    if (goal == counts) {
        answer++;
    }
    for (int i = 10; i < discount.size(); i++){
        counts[discount[i-10]]--;
        counts[discount[i]]++;

        if (counts[discount[i-10]] == 0){
            counts.erase(discount[i-10]);
        }

        if (goal == counts){
            answer++;
        }
    }

    return answer;
}
```

- 슬라이딩 윈도우 + 투포인터로 풀었는데 풀고나서 다른 풀이를 보니까 너무 효율적인게 있어서 변경했다.
- map을 단순히 == 연산자로 비교할 수 있는지 처음 알게 되었다.
  - map의 비교를 위해 value가 0이되면 erase로 바로 지워주었다.

## 의상

```c++
#include <string>
#include <vector>
#include <unordered_map>
#include <vector>

using namespace std;

void dfs(int index, vector<int>& counts, int& answer){
    if (index == counts.size()){
        answer++;
        return;
    }

    for (int _ = 0; _ < counts[index]; _++){
        dfs(index+1, counts, answer);
    }

    dfs(index+1, counts, answer);
}

int solution(vector<vector<string>> clothes) {
    int answer = 0;
    unordered_map<string, int> typeCounts;
    vector<int> counts;

    for (auto& cloth : clothes){
        typeCounts[cloth[1]]++;
    }

    for (auto& tc : typeCounts){
        counts.push_back(tc.second);
    }

    dfs(0, counts, answer);

    return answer-1;
}
```

- 조합으로 푸는거같은데 우선 완탐돌려봤더니 해결되었다.

# 총정리

- 오전 코드카타 + 챌린지반
- 오후 7번과제 필수과제까지

- map을 단순히 == 연산자로 비교할 수 있는지 처음 알게 되었다.
  - map의 비교를 위해 value가 0이되면 erase로 바로 지워주었다.

- 언리얼의 향상된 입력 시스템은 '서브시스템'이라는 매니저가 관리한다.
- 플레이어의 로컬 환경에서 이 매니저를 찾아야만 키 매핑을 추가하고 제거할 수 있다.
  - GetSubSystem
- 벡터 정규화로 대각 이동이 빠르던 버그를 수정했다.
  - 벡터의 크기를 1로 만듬
- AddActorLocalOffset의 두번째 인자는 bSweep
  - true : 충돌방해물이 있으면 멈춤, false : 그냥 진행
- 좌표 제어
  - Local Offset : 캐릭터가 바라보는 방향 기준
  - World Offset : 세계의 절대적인 X,Y,Z축 기준
- 입력과 월드의 좌표 차이
  - 입력기준 : X(가로, D(+)/A(-)키) , Y(세로, W(+)/S(-)키)
  - 월드기준 : X(Forward, 앞/뒤 -> W(+)/S(-)키), Y(Right, 오른쪽/왼쪽 -> D(+)/A(-)), Z축(Up, 위(+)/아래(-))
  - 입력의 Y를 월드의 X에 넣어주었다.

- 왼손 좌표계
  - 엄지 Z (Yaw의 축), 검지 X (Roll의 축), 중지 Y(Pitch의 축)
  - ![](https://velog.velcdn.com/images/kyu_/post/b3759bb3-3d2f-4662-be1e-2463b9422a07/image.png)

- ClampAngle
  - 각도를 특정 범위내에서 고정
  - 첫번째인자 : 입력
  - 두번째 인자 : 최소값
  - 세번째 인자 : 최대값
