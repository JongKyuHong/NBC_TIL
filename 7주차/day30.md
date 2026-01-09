# 팀프로젝트

## Actor의 라이프사이클

```c++
void AItem::PostInitializeComponents()
{
    Super::PostInitializeComponents();
    UE_LOG(LogSparta, Warning, TEXT("%s PostInitializeComponents"), *GetName());
}

void AItem::BeginPlay()
{
    Super::BeginPlay();

    UE_LOG(LogSparta, Warning, TEXT("%s BeginPlay"), *GetName());
}

void AItem::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
    // 틱함수는 로그를 넣지않는다.
}

void AItem::Destroyed()
{
    UE_LOG(LogSparta, Warning, TEXT("%s Destroyed"), *GetName());
    Super::Destroyed();
}

void AItem::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
    Super::EndPlay(EndPlayReason);
    UE_LOG(LogSparta, Warning, TEXT("%s EndPlay"), *GetName());
}
```

- 생명주기 코드에 로그를 찍어주었을때 생성자와 Destroyed는 레벨에 액터를 배치하자마자/삭제되자마자 뜨게됨
- Tick은 어차피 계속 실행되고 저기다가 로그를 두었을때 다른 로그들 확인이 불가하므로 틱에는 넣지않음

## Actor의 transform

### Transform

- 월드 좌표, Set Actor Location
- 상대좌표, 로컬좌표, Set relative location

```c++
void AItem::BeginPlay()
{
    Super::BeginPlay();

    SetActorLocation(FVector(300.0f, 200.0f, 100.0f));
    SetActorRotation(FRotator(0.0f, 90.0f, 0.0f));
    // pitch, yaw, roll
    // y축, z축, x축
    SetActorScale3D(FVector(2.0f));

    FVector NewLocation(300.0f, 200.0f, 100.0f);
    FVector NewRotation(0.0f, 90.0f, 0.0f);
    FVector NewScale(2.0f);

    FTransform NewTransform(NewRotation, NewLocation, NewScale);

    setActorTransform(NewTransform);
}
```

- 특이하게도 Rotator는 pitch, yaw, roll순으로 되어있다. 각각 y, z, x축 회전
- FTransform이라는 구조체형태를 통해 Location, Rotation, Scale을 한번에 지정해 줄 수 있다.

### Tick

```c++
void AItem::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    if (!FMath::IsNearlyZero(RotationSpeed))
    {
        AddActorLocalRotation(FRotator(0.0f, RotationSpeed * DeltaTime, 0.0f));
    }
}
```

- Tick함수는 계산을 최소화하는게 좋다 그래서 RotationSpeed가 0일때는 계산하지 않겠다는 방어코드를 넣을것인데 부동소수점 특징상 소수점끼리 계산은 완벽하게 정확하지는 않은데 이를 방지하는것을 언리얼에서 지원해준다.
- FMath::IsNearlyZero(), 0에 가깝다면 0으로 쳐준다 이런뜻
- PrimaryActorTick.bCanEverTick = true; 이걸 켜야만 Tick함수를 사용할 수 있다.
  - 평상시는 false로 주어서 꺼놓고 Tick을 사용할때만 true로 설정해주자.

## 클래스와 리플렉션 시스템

### 블루프린트와 C++구현에 대한 차이점

#### 블루프린트의 특징

- 빌드가 필요가없다.

- 프로그래머가 아닌분들도 쉽게 로직을 이해할 수 있다.

- 디자이너, 기획자가 사용

- UI, 시네마틱, 애니메이션 연출, 프로토타이핑 등에 사용

#### C++의 특징

- 난이도가 있다.

- 엔진자체가 C++로 만들어져있기 때문에 엔진의 거의 모든 부분을 바꾸고 속도최적화등 커스터마이징을 할 수 있다.

- 서드파티 라이브러리같은것들을 사용할 수 있다.

- 대규모작업에서 체계적으로 관리가능

- 메모리관리 직접 가능

- 하지만 빌드하는 과정이 번거롭다.

- 개발자

- 복잡한 계산, 게임 로직

### 리플렉션 시스템은 왜나왔을까?

- C++로 작업한것은 스태틱메시같은게 안보임
- 메시설정, 머티리얼 설정은 리플렉션으로 빼주면 좋다.

```c++
#include "Item.generated.h"  // 리플렉션 시스템을 위해 필요, 꼭 include의 가장 마지막에 위치

UCLASS() // 클래스를 리플렉션 시스템에 등록
class NBC1_API AItem : public AActor
{
	GENERATED_BODY() // 리플렉션 시스템에 등록하기 위한 코드

public:
	AItem();

protected:
    UPROPERTY() // 리플렉션에 아래 멤버변수를 리플렉션에 등록
	USceneComponent* SceneRoot;

    UPROPERTY()
	UStaticMeshComponent* StaticMeshComp;

    UPROPERTY()
	float RotationSpeed;

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
};
```

#### UCLASS()

- UCLASS(Blueprintable, BluePrintType) == UClASS() : 같은 의미
- Blueprinable은 이 클래스를 블루프린트에서 상속가능하다.
- BlueprintType은 블루프린트에서 이 클래스를 변수로 선언하거나 참조할 수 있음
- 둘다 Not 붙이기 가능

#### UPROPERTY()

- 리플렉션에 변수 등록
- VisibleAnywhere : 보이긴 하는데 수정 불가
- EditAnywhere : 수정 할 수 있음
- EditDefaultsOnly : 클래스 디폴트에서는 수정가능 인스턴스에서는 수정 불가
- EditInstanceOnly : 디폴트는 못바꾸고 인스턴스는 수정 가능

// 두번째인자, 블루프린트에서 노드를 만들 때

- BlueprintReadOnly : get만 가능
- BlueprintReadWrite : Get/Set가능

#### UFUNCTION()

- BlueprintCallable
- BlueprintPure : 값만 반환

## 복습

### Actor와 Object의 개념 이해하기

- Object의 개념

  - UObject는 언리얼 엔진에서 모든 클래스의 최상위 부모 클래스
  - 스스로 월드 레벨에 배치될 수 없음, 주로 데이터나 로직만 담당
  - 주로 화면에 보이지않는 데이터나 로직을 처리

- Actor의 개념
  - AActor는 UObject를 상속한 클래스로 월드에 배치할 수 있다.
  - 위치, 회전, 크기 등 공간적 정보를 가지고 있고, 여러 컴포넌트를 추가로 붙일 수 있다.
  - 실제 게임 세계에서 보이고 상호작용하는 캐릭터, 적 몬스터, 무기, 조명, 파티클 효과등은 주로 AActor를 기반으로 제작합니다.

## 공식문서에서 보는 Actor와 Object

### Actor

- 레벨에 배치할 수 있는 오브젝트
- 3D 트랜스폼을 지원하는 범용 클래스
- 액터는 어떤 의미에서 컴포넌트라고 하는 특별한 타입의 오브젝트를 담는 컨테이너라고 생각할 수 있다.
- 다양한 타입의 컴포넌트를 사용하여 액터의 이동 및 렌더링 방식등을 제어할 수 있다.
- 다른 주요 기능으로는 플레이 도중 네트워크를 통해 프로퍼티 및 함수호출을 리플리케이션 하는것
- UActorComponent : 베이스 컴포넌트, 액터의 일부로 포함될 수 있다. 원하면 틱하게 할 수 있다. 액터 컴포넌트는 특정 액터와 연결되어 있지만 월드의 특정 위치에 존재하지는 않고, AI또는 플레이어 입력 해석과 같은 개념적 기능에 사용됩니다.
- USceneComponent : 씬 컴포넌트는 트랜스폼이 있는 액터 컴포넌트입니다. 트랜스폼은 월드에서의 위치로 위치 회전 스케일로 정의됩니다. 씬 컴포넌트는 계층형 방식으로 서로 어태치될 수 있습니다. 액터의 위치 회전, 스케일은 계층구조의 루트에있는 씬컴포넌트에서 가져온다.
- UPrimitiveComponent : 프리미티브 컴포넌트는 메시나 파티클 시스템 같은 일종의 그래픽 표현이 있는 씬 컴포넌트

#### 리플리케이션

- 네트워크 멀티플레이어 게임을 처리할 때 월드 내 액터의 동기화 상태를 유지하는데 사용
- 프로퍼티 값과 호출을 모두 리플리케이트 할 수 있으므로, 모든 클라이언트에서 게임의 스테이트를 완벽하게 제어할 수 있음

### Object

#### UCLASS 매크로

- 언리얼 기반 타입을 설명하는 UCLASS에 대한 레퍼런스를 UObject에 제공
- UCLASS는 클래스 디폴트 오브젝트(CDO)라는 하나의 오브젝트를 유지
  - CDO : 기본적으로 클래스 생성자에 의해 생성되고 이후에는 수정되지 않는 디폴트 '템플릿' 오브젝트

#### UObject

- 모든 C++ UObject는 엔진 시작 시 초기화되며 엔진은 디폴트 생성자를 호출한다.
- UObject 생성자는 가벼워야 하고 디폴트값과 서브오브젝트를 구성하는데만 사용되어야함
- 생성시 다른 함수 기능을 호출해서는 안됨
- 액터및 액터 컴포넌트의 경우, 대신에 초기화 함수 기능을 BeginPlay()메서드에 넣어야한다.

## 숙제

```c++
// Fill out your copyright notice in the Description page of Project Settings.


#include "Item.h"
#include "Components/AudioComponent.h"

DEFINE_LOG_CATEGORY(JongKyu);

AItem::AItem()
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


	StaticMeshAudio = CreateDefaultSubobject<UAudioComponent>(TEXT("Audio"));
	StaticMeshAudio->SetupAttachment(SceneRoot);

	static ConstructorHelpers::FObjectFinder<USoundBase> AudioAsset(TEXT("/Game/Resources/Audio/Starter_Wind05.Starter_Wind05"));
	if (AudioAsset.Succeeded())
	{
		StaticMeshAudio->SetSound(AudioAsset.Object);
	}
}

void AItem::BeginPlay()
{
	Super::BeginPlay();

	UE_LOG(JongKyu, Warning, TEXT("%s BeginPlay"), *GetName());
}

void AItem::Tick(float deltaTime)
{
	Super::Tick(deltaTime);

	UE_LOG(JongKyu, Warning, TEXT("성능저하 실험입니다."));
}
```

- 코드 이렇게 짜놓고 왜 Tick돌면서 로그가 안찍히지? 이러고 있었다.
- 생각해보니까 Tick찍으려면 생성자에 PrimaryActorTick.bCanEverTick = true; 이거 써줘야함

# 코드카타

## 1.

- 체육복

```c++
#include <string>
#include <vector>
#include <unordered_map>
#include <algorithm>

using namespace std;

bool algo(unordered_map<int, int>& um, vector<int>& reserve, int l){
    auto findk3 = um.find(l);
    if (findk3 == um.end()){
        auto it3 = find(reserve.begin(), reserve.end(), l);
        if (it3 != reserve.end()){
            um.insert({l, 1});
            return true;
        }
    }
    return false;
}


int solution(int n, vector<int> lost, vector<int> reserve) {
    int answer = 0;
    unordered_map<int, int> um;
    sort(lost.begin(), lost.end());

    for (int r : reserve){
        auto findk3 = um.find(r);
        if (findk3 == um.end()){
            auto it3 = find(lost.begin(), lost.end(), r);
            if (it3 != lost.end()){
                um.insert({r, 1});
            }
        }
    }

    for (int l : lost){
        if (um.find(l) != um.end()){
            continue;
        }
        if (algo(um, reserve, l)){
            continue;
        }
        if (algo(um, reserve, l-1)){
            continue;
        }
        if (algo(um, reserve, l+1)){
            continue;
        }

        answer++;
    }

    return n-answer;
}
```

- 본래코드는 reserve에서 바로 지우는 방식이 기억이 안나서 이렇게 um이라고 따로 처리하는 map을 사용했다.

```c++
#include <string>
#include <vector>
#include <algorithm>

using namespace std;

int solution(int n, vector<int> lost, vector<int> reserve) {
    int answer = 0;
    sort(lost.begin(), lost.end());

    for (int& l : lost){
        auto it = find(reserve.begin(), reserve.end(), l);
        if (it != reserve.end()){
            reserve.erase(it);
            l = 0;
        }
    }

    for (int l : lost){
        if (l == 0){
            continue;
        }
        auto it = find(reserve.begin(), reserve.end(), l-1);
        if (it != reserve.end()){
            reserve.erase(it);
            continue;
        }
        auto it2 = find(reserve.begin(), reserve.end(), l+1);
        if (it2 != reserve.end()){
            reserve.erase(it2);
            continue;
        }
        answer++;
    }

    return n-answer;
}
```

- 찾아보니 이런식으로 it는 그냥 erase의 인자로 넘겨주면 삭제할 수 있더라..

```c++
for (int& l : lost){
    auto it = find(reserve.begin(), reserve.end(), l);
    if (it != reserve.end()){
        reserve.erase(it);
        l = 0;
    }
}
```

- 이 구문도 신기했다. &로 인해 원본에 접근이 가능하지 처리한 사람은 0으로 처리해버리기

## 2.

```
#include <string>
#include <vector>

using namespace std;

int solution(string s) {
    int answer = 0;

    char prev = '\0';
    int count = 0;
    int otherCount = 0;
    int index = 0;
    for (int i = 0; i < s.length(); i++){
        if (prev == '\0'){
            prev = s[i];
            count++;
        } else {
            if (prev != s[i]){
                otherCount++;
                if (count == otherCount){
                    count = 0;
                    otherCount = 0;
                    prev = '\0';
                    answer++;
                    index = i;
                }
            } else {
                count++;
            }
        }
    }

    if (count != 0){
        answer++;
    }

    return answer;
}
```

- char의 경우 \0로 초기화를 많이 한다고 한다.

# 오늘 총평

## UE5 C++

- Actor 라이프사이클에서 `PostInitializeComponents / BeginPlay / Tick / EndPlay / Destroyed` 타이밍을 로그로 확인해봄.
- Transform 다루는 법 정리: `SetActorLocation/Rotation/Scale`, 그리고 `FTransform`으로 한 번에 세팅하는 방식 복습.
- Tick은 매 프레임이라 성능에 민감 → 로그를 Tick에 넣으면 Output Log가 도배되니 보통 피함.
- Tick이 안 도는 이유를 직접 겪음: 생성자에서 `PrimaryActorTick.bCanEverTick = true;`를 켜야 Tick이 호출됨.
- 리플렉션 시스템(UCLASS/UPROPERTY/UFUNCTION)의 목적: C++로 만든 것들을 에디터/블루프린트에서 보이고 수정 가능하게 하려면 매크로로 등록이 필요하다는 점 정리.
- `UAudioComponent` 붙이기/사운드 세팅하면서 헤더 include(incomplete type), `USoundBase`로 에셋 로드 후 `SetSound()`로 연결하는 흐름을 익힘.

## 코드카타

- 체육복 : 처음엔 `unordered_map`으로 “사용 처리”를 했지만, 나중엔 `find`로 iterator 얻어서 `reserve.erase(it)`로 지우는 방식이 더 직관적이라는 걸 알게 됨.
- range-for에서 `int& l`처럼 **참조로 받으면 원본 벡터 값을 바꿀 수 있음**
- 문자열 나누기: 센티넬로 `char prev = '\0'`를 두고, 기준 문자 카운트와 다른 문자 카운트를 맞추는 방식으로 풀이.
