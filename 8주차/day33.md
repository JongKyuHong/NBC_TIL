![](https://velog.velcdn.com/images/kyu_/post/8c70682c-4909-49a5-8991-450ee17a0e23/image.png)

# 언리얼 C++ 강의

## 4-1강

### UMG

#### HUD란?

- 게임 내에서 플레이어에게 정보를 제공하기 위한 화면
- 플레이어가 현재 상황을 이해하는데 필요한 체력, 미니맵, 퀘스트 업데이트와 같은 데이터 제공

- Canvas기반 HUD

  - AHUD클래스를 상속하여 구현
  - 기본적인 2D그리기 작업
  - 간단한 HUD에 적합

- UMG(Unreal Motion Graphics)
  - 언리얼의 Widget Blueprint를 이용한 UI 시스템
  - 더 직관적임

#### 위젯 블루프린트?

- 언리얼엔진에서 UI를 시각적으로 설계할 수 있도록 제공하는 에디터용 블루프린트
- 개발자는 다양한 UI요소를 간편하게 배치 가능
- Text Block, Button, Progress Bar등

## 4-3강

### Widget Component

- UMG로 만든 위젯을 3D월드에 붙일 수 있게 해주는 컴포넌트
- 2D로만 보이던 UI를 공간 내 특정 위치에 붙이고 카메라 각도에 따라 회전하거나 크기가 달라지는 모습을 만들 수 있음
- Actor에 부착할 수 있는 컴포넌트

## 4-4강

### 파티클 시스템

- 게임 내에서 불꽃, 연기, 폭발, 먼지 등 다양한 시각적 효과를 구현하기 위한 도구
- 파티클은 다수의 작은 '입자'들이 모여 움직이면서 특정한 모양, 색상, 혹은 애니메이션 효과를 만들어낸다.
- Cascade vs Niagara
- Cascade
  - 오래된 파티클 편집 툴
  - 초급자가 배우기에 상대적으로 간단하고 빠르게 결과를 볼 수 있다.
- Niagara

  - 언리얼 4후반부터 새롭게 도입, 5부터는 공식적으로 권장하는 방식
  - 모듈단위로 다양한 파티클 동작 정교하게 제어
  - 블루플린트나 머티리얼, 스크립팅과 유기적으로 연동
  - GPU 파티클, 신규기능 업데이트가 빠르게 적용

- 파티클은 반복 생성 시 부하가 커질 수 있다.
- 불필요하게 많은 파티클이 생성되지 않도록 해야함
- 필요에따라 Looping옵션을 끄거나 키자
- 아이템이 이동/회전하는 동안 파티클도 움직여야 한다면 `SpawnEmitterAttached()`를 사용해 부착 대상 컴포넌트를 지정해줘야 한다.

### 사운드 효과

- 사운드 웨이브
  - 한개의 오디오 파일을 뜻함
  - 단일 오디오파일을 그대로 재생하는 경우 사용
- 사운드 큐
  - 오디오 편집 그래프 시스템
  - 복잡한 사운드 로직 구성 가능
  - 여러 사운드 웨이브를 조합하거나 고급기능을 적용해야할때 사용

# 강의 복습

## 1-6숙제

```c++
#include "TestActor.h"

ATestActor::ATestActor()
{
	SceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("SceneRoot"));
	SetRootComponent(SceneRoot);

	StaticMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	StaticMeshComp->SetupAttachment(SceneRoot);

	static ConstructorHelpers::FObjectFinder<UStaticMesh> MeshAsset(TEXT("/Game/Resources/Props/SM_Chair.SM_Chair"));
	if (MeshAsset.Succeeded())
	{
		StaticMeshComp->SetStaticMesh(MeshAsset.Object);
	}

	static ConstructorHelpers::FObjectFinder<UMaterial> MaterialAsset(TEXT("/Game/Resources/Materials/M_Metal_Gold.M_Metal_Gold"));
	if (MaterialAsset.Succeeded())
	{
		StaticMeshComp->SetMaterial(0, MaterialAsset.Object);
	}


	PrimaryActorTick.bCanEverTick = true;
	RotationSpeed = 90.0f;
	SpeedZ = 200.0f;
	RunningTime = 0.0f;
	isBig = false;
}

void ATestActor::BeginPlay()
{
	Super::BeginPlay();

	SetActorLocation(FVector(0.0f, 0.0f, 0.0f));
	SetActorRotation(FRotator(0.0f, 0.0f, 0.0f));
	SetActorScale3D(FVector(2.0f));
}

void ATestActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	AddActorLocalOffset(FVector(0.0f, 0.0f, SpeedZ*DeltaTime));
	AddActorLocalRotation(FRotator(0.0f, RotationSpeed * DeltaTime, 0.0f));

	RunningTime += DeltaTime;

	if (RunningTime >= 2.0f)
	{
		RunningTime = 0.0f;
		if (isBig)
		{
			SetActorScale3D(FVector(2.0f));
			isBig = false;
		}
		else
		{
			SetActorScale3D(FVector(5.0f));
			isBig = true;
		}

	}
}
```

![](https://velog.velcdn.com/images/kyu_/post/3fa047aa-31f9-4355-8a49-e4b1fb9eac17/image.png)

- 계속 하늘로 올라가면서 커졌다가 작아졌다가를 반복했다.

## 2강

## 2-1

### StaticClass()

```c++
ATestGameMode::ATestGameMode()
{
    DefaultPawnClass = ATestCharacter::StaticClass();
}
```

- GameMode의 생성자에서 이렇게 DefaultPawnClass를 초기화해준다.
  - DefaultPawnClass는 상위에서 선언되어있는 기본적으로 어떤 Pawn을 스폰할것이냐를 말함
- ATestCharacter::StaticClass()
  - StaticClass는 언리얼 리플렉션 시스템에서 제공하는 정적 메서드로 앞에 붙은 클래스의 메타데이터를 담고있는 UClass 객체를 반환한다.

### 숙제

- 카메라 위치 변경 해보기

```c++
SpringArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
SpringArmComp->SetupAttachment(RootComponent);
SpringArmComp->TargetArmLength = 400.0f;
SpringArmComp->SocketOffset = FVector(0.0f, 0.0f, 200.0f);
SpringArmComp->bUsePawnControlRotation = true;

CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
CameraComp->SetupAttachment(SpringArmComp, USpringArmComponent::SocketName);
CameraComp->SetRelativeRotation(FRotator(-15.0f, 0.0f, 0.0f));
CameraComp->bUsePawnControlRotation = false;
```

- Pawn vs Character 개념

```
AActor -> APawn -> ACharacter
```

- AActor : 월드에 배치 가능한 기본 객체
- APawn : 플레이어가/AI가 빙의 할 수 있는 액터, 기본적으로 MovementComponent없음
- ACharacter : Pawn + CharacterMovementComponent + CapsuleComponent + SkeletalMeshComponent가 기본

```c++
MoveForward = 1.0f; // 생성자에서 초기화

void ATestPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	AddActorLocalOffset(FVector(0.0f, MoveForward, 0.0f));

}

```

![](https://velog.velcdn.com/images/kyu_/post/9076bdb2-663c-4b8e-93c0-3d8c1181593a/image.gif)

- tick당 1.0f씩 이동하는 Pawn을 만들었다.

# 코드카타

## 최소공배수, 최대공약수

- [N개의최소공배수](https://school.programmers.co.kr/learn/courses/30/lessons/12953)

```c++
int gcd(int a, int b){
    while (b!=0){
        int r = a % b;
        a = b;
        b = r;
    }
    return a;
}

int lcm(int a, int b){
    return (a*b)/gcd(a, b);
}
```

- 어차피 numeric헤더에 있지만 그래도 한번 구현해보자

## 문자열 다루기

- [JadenCase 문자열 만들기](https://school.programmers.co.kr/learn/courses/30/lessons/12951)

문자열 관련 기능들을 한번 정리해서 기억해야 할 것 같다.

- 길이/상태

  - s.length() : 길이
  - s.empty() : 비어있는지
  - s.clear() : 비우기

- 접근

  - s[i] : 인덱스 접근
  - s.at(i) : 인덱스 접근
  - s.front, s.back() : 첫/끝 접근

- 추가/삭제/수정

  - s+=t, s.append(t) : 뒤에 붙이기
  - s.push_back(ch), s.pop_back() : 문자 1개 추가/삭제
  - s.insert(pos, t) : pos앞에 삽입
  - s.erase(pos, count) : 삭제
  - s.replace(pos, count, t) : 구간 치환

- 탐색/부분문자열
  - s.find(t) : 찾으면 인덱스, 아니면 string::npos
  - s.rfind(t) : 뒤에서부터 찾기
  - s.substr(pos, count) : 부분 문자열 추출
- 비교

  - s == t, s < t : 사전순 비교
  - s.compare(t) : 같으면 0 / 작으면 음수 / 크면 양수

- 분류

  - isalpha(s)
  - isdigit(s)
  - isspace(s)
  - isupper(s)
  - islower(s)

- 변환
  - toupper(s)
  - tolower(s)

# 오늘 총 정리

1. UMG / HUD

- HUD: 플레이어에게 체력/미니맵/퀘스트 같은 정보를 보여주는 화면
- Canvas HUD: AHUD 상속, 2D 그리기 중심(간단한 HUD)
- UMG: Widget Blueprint 기반 UI 시스템(시각적으로 구성)

2. 위젯 블루프린트

- UI를 에디터에서 배치/조립하는 블루프린트
- TextBlock / Button / ProgressBar 같은 위젯으로 구성

3. Widget Component

- UMG 위젯을 3D 월드(액터)에 붙여서 보여주는 컴포넌트
- 카메라 각도/거리 따라 보이는 방식이 달라짐(월드 UI)

4. 파티클 시스템

- 불꽃/연기/폭발 같은 VFX(입자 기반 효과)
- Cascade(구형, 단순) vs Niagara(신형, UE5 권장, 모듈 기반 제어)
- 반복 생성/루프는 성능 부담 → 필요 시 Loop 조절
- 움직이는 액터에 “붙여서” 같이 움직이려면 SpawnEmitterAttached() 고려

5. 사운드 효과

- Sound Wave: 단일 오디오 파일 재생
- Sound Cue: 그래프 기반 사운드 구성(조합/랜덤/효과 등 로직 가능)

6. Tick 실습(Actor Transform)

- AddActorLocalOffset + DeltaTime으로 초당 이동 구현
- AddActorLocalRotation + DeltaTime으로 초당 회전 구현
- 2초마다 스케일 변화(현재 코드는 토글 방식, Sin 쓰면 부드럽게 가능)

7. GameMode / DefaultPawnClass / StaticClass

- GameMode에서 DefaultPawnClass로 `기본 스폰 Pawn` 지정
- StaticClass()는 해당 클래스의 UClass(클래스 메타데이터) 반환

8. SpringArm / Camera 세팅

- TargetArmLength: 카메라 거리
- SocketOffset: 어깨너머/상단 등 카메라 위치 오프셋 실험
- bUsePawnControlRotation 설정에 따라 회전 적용 방식이 달라짐

9. Pawn vs Character

- 상속: AActor -> APawn -> ACharacter
- Pawn: Possess 가능, 기본 이동 컴포넌트 없음(직접 이동 구현하기 좋음)
- Character: 캐릭터 이동 구성요소가 기본 포함(이동 로직이 더 “완성형”)

10. 코드카타

- gcd/lcm: 유클리드 호제법으로 최대공약수, a\*b/gcd로 최소공배수
- 문자열: substr, find, erase, replace 등 “부분 문자열 가공” 감 익히기
