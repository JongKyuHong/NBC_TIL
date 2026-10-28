# 무기 장착/해제와 물리 상태 관리

캐릭터가 검을 들고 있을때와 바닥에 떨어진 검은 움직이는 방식이 다름

- 장착 중 : 캐릭터 손의 위치를 따라 움직여야 함
- 바닥에 있을 때 : 중력과 충돌을 통해 움직여야 함

Unreal에서는 이 차이를 컴포넌트 부착과 물리 시뮬레이션으로 처리할 수 있음

캐릭터의 오른손 소켓에 검을 부착한다고 해보자, Socket은 손이나 등처럼 무언가를 부착하기 위해 미리 지정하는 위치를 말한다. 공격 애니메이션으로 손이 움직이면 연결된 무기도 따라 움직일 수 있다.

```c++
void EquipWeapon(AWeapon* Weapon, ACharacter* Character)
{
    if (!Weapon || !Character) return;

    UStaticMeshComponent* WeaponMesh = Weapon->GetWeaponMesh();

    // 장착 중에는 물리 시뮬레이션 비활성화
    WeaponMesh->SetSimulatePhysics(false);
    WeaponMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

    // 캐릭터의 손 소켓에 부착
    Weapon->AttachToComponent(
        Character->GetMesh(),
        FAttachmentTransformRules::SnapToTargetIncludingScale,
        TEXT("WeaponSocket")
    );
}
```

- SetSimulatePhysics(false) : 물리시뮬레이션 중단
- SetCollisionEnabled() : 충돌 처리방식 변경
- AttachToComponent() : 다른 컴포넌트의 자식으로 연결
  SnapToTargetNotIncludingScale은 대상 Socket의 위치와 회전에 맞추되, 스케일까지는 그대로 맞추지 않는 규칙

무기를 버릴때는 반대로 하면 됨

```c++
Weapon->DetachFromActor(
    FDetachmentTransformRules::KeepWorldTransform
);

WeaponMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
WeaponMesh->SetSimulatePhysics(true);
```

KeepWorldTransform를 사용하면 손에서 분리하는 순간의 월드 위치를 유지할 수 있고, 이후 물리시뮬레이션을 활성화해서 검이 중력의 영향을 받으며 떨어질 수 있다.

# 일시정지

언리얼에서 게임을 일시정지하면 무엇이 멈추고, 무엇은 계속 동작해야 할까?
ESC를 눌러서 일시정지 메뉴를 열었다고 생각해보자

1. 게임 일시정지의 기본 원리

- 플레이어와 몬스터는 움직이지 않아야 함
- 몬스터의 공격이나 스킬 쿨타임도 멈춰야 함
- 하지만 일시정지 메뉴의 버튼은 클릭할 수 있어야 함
- 다시 시작 버튼을 누르면 게임이 정상적으로 재개되어야 함

UGameplayStatics::SetGamePaused()를 사용할 수 있음

```c++
#include "Kismet/GameplayStatics.h"

// 게임 일시정지
UGameplayStatics::SetGamePaused(this, true);

// 게임 재개
UGameplayStatics::SetGamePaused(this, false);
```

2. 일시정지중에도 실행해야 하는 기능

예를들어 일시정지 상태에서도 특정 Actor의 Tick을 실행해야 한다면

```c++
SetTickableWhenPaused(true);
```

일시정지 되어도 AActor가 Tick을 받을 수 있게 하는 설정
Tick이 실행된다고 해서 게임 시간까지 정상적으로 흐르는것은 아니다. 일시정지 중에는 게임 시간 기반 DeltaTime이 0일 수 있다.

GetWorld()->GetTimeSeconds() : 일시정지하면 멈추는 게임 시간
GetWorld()->GetRealTimeSeconds() : 일시정지와 관계없이 흐르는 실제 시간

3. 일시정지시 메뉴조작

```c++
void AMyPlayerController::PauseGame()
{
    UGameplayStatics::SetGamePaused(this, true);

    bShowMouseCursor = true;

    FInputModeUIOnly InputMode;
    SetInputMode(InputMode);
}
```

- bShowModeUIOnly : 마우스 보이게
- FInputModeUIOnly : 입력을 UI조작 중심으로 처리
- SetInpuMode : 해당 입력 모드 적용

재개의 경우 반대로

```c++
void AMyPlayerController::ResumeGame()
{
    UGameplayStatics::SetGamePaused(this, false);

    bShowMouseCuror = false;
    FInputModeGameOnly InputMode;
    SetInputMode(InputMode);
}
```

주의할점은 UIOnly상태에서는 기존 Gameplay입력으로 연결한 ESC키가 그대로 동작하지 않을 수 있다. 이 경우 메뉴 Widget에서 ESC입력을 처리하거나, 일시정지 중에도 처리되는 입력 경로를 따로 구성해야함

SetGlobalTimeDilation은 게임 시간의 진행속도를 조절하는 기능

```c++
UGameplayStatics::SetGlobalTimeDilation(this, 0.2f);
```

# 다수 캐릭터의 애니메이션 성능 최적화

1. 애니메이션은 왜 CPU를 사용하는가?

```
몬스터의 현재 상태 확인
        ↓
Animation Blueprint 업데이트
        ↓
재생할 애니메이션 결정
        ↓
여러 애니메이션 혼합 (Blend)
        ↓
각 Bone의 위치·회전 계산
        ↓
Skeletal Mesh에 결과 반영
```

이 과정에서 CPU는 Animation Blueprint와 애니메이션 상태/포즈 평가 등의 작업을 수행하고, GPU도 스키닝과 렌더링 작업을 담당할 수 있음

몬스터가 1마리면 부담이 적어도 300마리가 각각 복잡한 애니메이션을 계산한다면 비용이 상당해질 수 있음, 핵심은 모든 몬스터가 매 프레임 똑같은 수준으로 애니메이션을 계산할 필요는 없다는 것

2. 첫번째 방법 - 보이지 않는 캐릭터의 계산 줄이기

Visibility Based Anim Tick Option을 통해서 가능

- Always Tick Pose and Refresh Bones : 보이지 않아도 애니메이션 업데이트 Bone 변환 갱신
- Always Tick Pose : 애니메이션은 계속 업데이트하되, 보이지 않을 때 Bone 갱신 생략
- Only Tick Pose When Rendered : 렌더링될 때만 애니메이션 포즈 업데이트
- Only Tick Montages When Not Rendered : 보이지 않을 때 일반 포즈 갱신은 생략하고 Montage 진행은 유지

```c++
GetMesh()->VisibilityBasedAnimTickOption = EVisibilityBasedAnimTickOption::OnlyTickPoseWhenRendered;
```

이렇게 설정하면 보이지 않는 캐릭터의 불필요한 애니메이션 계산을 줄일 수 있다.
하지만 주의점은 화면에서 보이지 않는다고 해당 몬스터의 Gameplay 로직까지 멈춰도 된다는것은 아님
예를 들어 화면 밖의 몬스터도 공격하거나 사망할 수 있어야 한다면, 애니메이션 업데이트를 생략하면 AnimNotify에만 의존하는 공격 판정이 누락되지 않는지 주의해야 함

우선 메서드들을 다 알필요는 없고 이런게 존재한다는것만 알고 있자

3. 두 번째 방법 - 멀리 있는 캐릭터는 적게 업데이트하기

60FPS게임일때 가까운 몬스터는 초당 60회, 중간은 30회, 멀리있으면 15회 정도로 갱신
Unreal에서는 이를 Update Rate Optimization(URO)라고 부름
Skeletal Mesh Component의 Details패널에서 `Optimization -> Enable Update Rate Optimizations` 설정을 활성화 할 수 있음

4. 세 번째 방법 - 전체 애니메이션 계산 예산 관리

Unreal의 Animation Budget Allocator는 애니메이션에 사용할 CPU 시간 예산을 정하고, 캐릭터의 중요도에 따라 계산량을 조절

- 높은 중요도 : 플레이어와 교전 중
- 중간 중요도 : 주변 NPC
- 낮은 중요도 : 멀리있는 NPC

이 기능은 플러그인을 활성화하고 `USkeletalMeshComponentBudgeted`를 활용해 구성가능
Budget Allocator가 관리하는 컴포넌트는 기존 URO를 별도로 함께 적용하는 대신 예산 관리 시스템을 통해 업데이트가 조정

5. Animation Sharing

여러 캐릭터가 계산된 애니메이션 포즈를 공유하도록 최적화
다수의 캐릭터가 동일한 애니메이션 상태를 사용하는 군중 게임에서 특히 도움이 될 수 있음

5. 실제 게임에서는 어떻게 접근?

- 프로파일링 : Unreal Insights, Animation Insights, stat anim등을 통해 실제로 애니메이션 비용이 큰지 확인
- 보이지 않는 NPC : 불필요한 애니메이션 포즈 갱신 줄임
- 먼 거리 NPC : URO등으로 업데이트 빈도 낮춤
- 수백마리 이상 : Animation Budget Allocator, Animation Sharing을 검토
- 검증 : 전투 판정, Montage, Bone위치, 시각적 품질에 문제가 없는지 확인

GPU의 Skeletal Mesh 렌더링 비용이 문제라면 애니메이션 계산 최적화 외에도 Mesh LOD와 드로우 비용등을 별도로 분석 해야됨

# GetAllActorsOfClass

게임 월드에 몬스터가 100마리 있는 상황
그중 `AEnemyCharacter` 클래스에 해당하는 몬스터를 전부 찾아야 하는 상황일때 사용하는 함수

```c++
UGameplayStatics::GetAllActorsOfClass()
```

현재 월드에 존재하는 특정 클래스의 액터들을 찾아 배열에 담아주는 함수, 해당 클래스의 자식 클래스 액터도 검색 대상에 포함됨

```c++
static void GetAllActorsOfClass(
    const UObject* WorldContextObject,
    TSubclassOf<AActor> ActorClass,
    TArray<AActor*>& OutActors
);
```

- WorldContextObject : 어느 월드에서 찾을지 판단할 객체, 액터 내부라면 보통 this
- ActorClass : 검색할 액터 클래스
- OutActors : 검색된 액터를 저장할 배열

반환값은 void인게 OutActors에 검색결과가 담김

1. 조심할점 - 성능

```c++
void AEnemyManager::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    TArray<AActor*> Enemies;

    UGameplayStatics::GetAllActorsOfClass(
        this,
        AEnemyCharacter::StaticClass(),
        Enemies
    );
}
```

언리얼에서는 액터 검색에 내부 최적화를 사용하지만 틱에다가 두면 많이 느려질 수 있다.
따라서 매 프레임 모든 몬스터를 찾는 방식은 가능하면 피하는게 좋다.

2. 어떻게 개선할까?

게임 시작시 한번 검색하고, 결과를 보관해 재사용하는 방법이 있을 수 있다. 몬스터가 생성될때 AEnemyManager에 등록하고, 제거될때 등록 해제하는 방식을 사용할수도 있다.

3. 비슷한 API비교

- GetActorOfClass : 해당 클래스의 액터 하나를 검색
- GetAllActorsOfClass : 해당 클래스의 모든 액터를 배열로 검색
- TActorIterator : 조건에 맞는 액터를 순회

# GameplayTags

게임에 수십개의 상태가 있으면 이를 쉽게 관리하기 위해서 사용한다. GameplayTags는 속성이나 상태를 계층적인 이름으로 표현하는 기능
GAS와 시너지가 좋다.

# Enhanced Input Mapping Context

Input Mapping Context는 현재 상황에 적용할 키와 액션의 연결정보를 말함
그냥 캐릭터 상태와 차량에 탔을때 키에 따라 행동이 달라지듯이

- UInputAction : 점프, 이동, 상호작용 같은 행동을 정의
- UInputMappingContext : 특정 키를 어떤 Input Action에 연결할지 정의
- UEnhancedInputLocalPlayerSubsystem : 로컬 플레이어에게 MappingContext를 적용하고 관리

InputAction은 뭘할지, MappingContext는 어떤 입력으로 실행할지를 담당함

```
IMC_OnFoot
WASD -> 이동
E -> 상호작용
Space -> 점프

IMC_Vehicle
WASD -> 차량조작
E -> 차량에서 내리기
Space -> 핸드브레이크
```

Context는 주로 어떤 작업을 수행하거나 판단할 때 필요한 주변 상황이나 정보로 생각하면 된다.

# SpawnActor<T>()

```c++
T* SpawnActor<T>(
    UClass* Class,
    const FVector& Location,
    const FRotator& Rotation,
    const FActorSpawnParameters& SpawnParameters
);
```

Class는 어떤 액터 클래스를 만들지, SpawnParameters는 Owner, 생성시 충돌 처리 등 추가 설정
스포너를 운용할때 충돌로 인해 스폰이 실패하는 경우도 있다.
SpawnCollisionHandlingOverride여기서 충돌 여부와 관계에 따라 소환/미소환/근처에 소환시도하고 실패하면 미소환하는 옵션을 선택할 수 있다.

# Destroy, Endplay와 메모리

Destroy는 말그대로 파괴하는거 이제 월드에서 퇴장하라고 명령하는 함수
하지만 이때 메모리에서 즉시 해제되는건 아니다.
추후에 GC에서 회수해감

Endplay는 액터가 월드에서 제거되거나 플레이가 종료될 때 호출되는 수명 주기 함수
몬스터가 월드에서 퇴장하는 과정에서 떠나기전에 정리하는 함수라고 보면 됨

# 컴포넌트 붙이기

1. 생성

- CreateDefaultSubobject
  객체 생성, 생성자에서 기본 구성 요소 만들때 사용

- NewObject
  객체 생성, 게임 실행 중 필요한 UObject 만들 때 사용

2. 부착

- SetupAttachment
  부모 컴포넌트 지정하는 역할, 컴포넌트가 등록되기 전 사용
  등록되기 전에 어디에 붙을지 미리 정하기

- AttachToComponent
  컴포넌트 부착/재부착, 주로 게임 실행중에 사용
  이미 존재하는 컴포넌트를 게임 중 부착하거나 다시 붙이기

- RegisterComponent
  컴포넌트 월드 등록, 런타임에 새 컴포넌트 생성했을때 사용

# 직렬화

메모리에 존재하는 데이터를 저장하거나 전송할 수 있는 형태로 변환하는 과정

```c++
USTRUCT()
struct FWeaponData
{
    GENERATED_BODY()

    UPROPERTY()
    int32 Level = 5;

    UPROPERTY()
    float Damage = 25.5f;
};
```

이런 구조체가 있다고 했을때 서버는 이 데이터를 클라이언트에 전달해야 함

```
서버 메모리
-> 직렬화
네트워크 전송용 데이터
-> 역직렬화
클라이언트 메모리
```

메모리를 그대로 보내면 패딩이나 포인터 주소처럼 전송할 필요가 없거나 다른 pc에서 의미없는 정보까지 포함될 수 있다.
전송에 필요한 데이터를 정해진 규칙으로 변환하고, 수신측에서 다시 복원

## 직렬화는 언제 사용할까?

Unreal에서 UPROPERTY(Replicated)로 데이터를 복제하면 내부적으로 직렬화가 수행됨
일반적인 변수나 구조체는 엔진이 기본 직렬화를 하는데, 전송 형식을 최적화하거나 특별한 규칙이 필요하면 직접 구현할 수 있다.

# UFUNCTION

자주헷갈리는 4가지

- BlueprintCallable
  C++에서 구현 O, BP에서 구현/재정의 X

- BlueprintPure
  C++에서 구현 O, BP에서 구현/재정의 X

- BlueprintImplementableEvent
  C++에서 구현 X, BP에서 구현/재정의 O

- BlueprintNativeEvent
  C++에서 구현 O(기본 구현), BP에서 구현/재정의 O(선택적 재정의)

## BlurpintCallable - 실행 시키는 함수

```c++
// .h
UFUNCTION(BlueprintCallable)
void TakeDamage(int32 Damage);
// .cpp
void AMyCharacter::TakeDamage(int32 Damage)
{
    Health -= Damage;
}
```

블루프린트에서 이 함수를 호출하면 체력을 감소시킬 수 있음

## BlueprintPure - 값을 가져오는 함수

```c++
//. h
UFUNCTION(BlueprintPure)
int32 GetHealth() const;
// .cpp
int32 AMyCharacter::GetHealth() const
{
    return Health;
}
```

이 함수는 체력을 변경하지 않고 현재 체력을 반환함
Pure함수는 실행 핀 없이 값을 계산하거나 조회한다라고 기억하면 된다.

## BlueprintImplementableEvent

```c++
// .h
UFUNCTION(BlueprintImplementableEvent)
void PlayHitEffect();
```

C++에서는 함수의 몸체를 구현하지 않고, BP에서 이펙트 재생 동작등을 구현함

## BlueprintNativeEvent

```c++
// .h
UFUNCTION(BlueprintNativeEvent)
void OnHit();

virtual void OnHit_Implementation();
// .cpp
void AMyCharacter::OnHit_Implementation()
{
    // C++ 기본 동작
}
```

BP에서 재정의하지 않으면 C++기본구현이 실행됨, BP에서 재정의한 경우 부모 구현을 호출할지는 별도로 결정할 수 있음

# BindWidget

```c++
UPROPERTY(meta=(BindWidget))
TObjectPtr<UButton> BuyButton;
```

UI의 디자인은 Widget Blueprint에서 만들고, 버튼 클릭 처리는 C++에서 구현하려고 할때 이 둘을 연결해주는게 BindWidget이다.
Widget Blueprint에 이름이 BuyButton이고 타입이 호환되는 버튼을 만들어두면, 해당 위젯 인스턴스가 초기화될 때 C++멤버와 연결됨

- BindWidget : 해당 이름/타입의 위젯 연결 요구
- BindWidgetOptional : 해당 위젯이 없어도 허용

## NativeOnInitialized와 NativeConstruct차이

- NativeOnInitialized()
  UUserWidget 인스턴스 초기화 시 한 번
  버튼 클릭 이벤트처럼 한 번만 연결할 것

- NativeConstruct()
  내부 Slate Widget 구성 시 호출, 재호출 가능
  화면 표시 시 상태 갱신

- NativeDestruct()
  Slate Widget의 종료 과정
  화면 사용 기간에 맞춘 이벤트 연결 해제

```c++
UPROPERTY(meta = (BindWidget))
TObjectPtr<UButton> BuyButton;

virtual void NativeOnInitialized() override;
virtual void NativeConstruct() override;

UFUNCTION()
void HandleBuyClicked();
```

```c++
void UShopWidget::NativeOnInitialized()
{
    Super::NativeOnInitialized();

    if (BuyButton)
    {
        BuyButton->OnClicked.AddDynamic(
            this,
            &UShopWidget::HandleBuyClicked
        );
    }
}

void UShopWidget::NativeConstruct()
{
    Super::NativeConstruct();

    RefreshShopUI();
}
```

BuyButton은 OnInitialized에서 한번 연결하고, 상점UI를 갱신해야 할때 NativeConstruct 호출

# Unreal Interface

언리얼에서의 인터페이스는 보통 두 클래스가 함께 등장한다.

```c++
UINTERFACE(Blueprintable)
class UInteractable : public UInterface
{
    GENERATED_BODY()
};

class IInteractable
{
    GENERATED_BODY()

public:
    UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
    void Interact(AActor* Interactor);
};
```

- UInteractable : 언리얼 리플렉션 시스템에서 인터페이스를 나타내는 타입, 언리얼 엔진이 인터페이스를 인식하도록 하는 리플렉션용 타입이다.
- IInteractable : 실제 인터페이스 함수들을 선언하는 C++ 타입

```c++
UCLASS()
class AChest : public AActor, public IInteractable
{
    GENERATED_BODY()

public:
    virtual void Interact_Implementation(
        AActor* Interactor
    ) override;
};
```

여기서 AChest가 액터이면서 IInteractable것도 구현한다는 뜻이다.
대신에 함수명이 인터페이스에서는 Interact()라고 선언했는데, 실제 구현은 Interact_Implementation()이라고 작성함 이유는 Interact에서 BlueprintNativeEvent를 사용했음
C++에서도 기본 동작을 구현할 수 있고, Blueprint에서도 동작을 재정의할 수 있게 하는 기능이다.
언리얼은 이를 위해 두가지를 구분한다.

- Interact() : 언리얼이 관리하는 이벤트 호출 경로, 함수를 호출할때 C++구현을 실행할지, 블루프린트에서 재정의한 구현을 실행할지 언리얼이 결정
- Interact_Implementation() : C++에서 작성하는 실제 구현

## Execute\_

우리가 원하는 동작은 NPC든 문이든 상자든 동일한 방식으로 호출하면서 C++과 Blueprint중 실제로 구현된 함수가 실행되도록 하는 것

```c++
IInteractable::Execute_Interact(Target, this);
```

이 함수는 우리가 인터페이스에 직접 선언한 함수가 아님, UFUNCTION(BlueprintNativeEvent)로 선언된 인터페이스 함수에 대해 언리얼이 만들어 준것

Target : 인터페이스를 구현한 객체, this : 선언한 Interact(AActor\* Interactor)의 매개변수
이 함수는 대상의 실제 구현을 찾아서 호출함
만약에 Implementation()을 직접 호출하면 Blueprint에서 재정의한 동작을 거치지 않을 수 있어서 Execute\_를 통해 호출하는것이다

대신 Execute\_는 Blueprint에서도 구현/재정의 할 수 있는 인터페이스 함수에서만 생성된다.

# UAnimInstance와 UAnimMontage

- UAnimInstance : 캐릭터의 애니메이션을 관리하고 재생 상태를 제어하는 객체 = 영상을 재생하고 제어하는 플레이어
- UAnimMontage : 공격/스킬/재장전처럼 특정 동작을 재생하기 위한 애니메이션 에셋 = 재생할 영상 파일

```c++
UAnimInstance* AnimInstance =
    GetMesh()->GetAnimInstance();
```

캐릭터가 공격 몽타주를 재생하려면 UAnimInstace를 통해 실행할 수 있다.

```c++
UAnimInstance* AnimInstance =
    GetMesh()->GetAnimInstance();

if (!AnimInstance || !AttackMontage)
{
    return;
}

float Duration = AnimInstance->Montage_Play(
    AttackMontage
);
```

Montage_Play()는 몽타주 재생을 시도하는 함수, 몽타주 길이를 반환, 재생에 실패하면 `.0f`를 반환

## 애니메이션이 끝났다는걸 어떻게 알까?

`Montage_SetEndDelegate()`를 통해 알 수 있다.
몽타주가 실제로 종료되었을때 호출되는 콜백이다.

공격 도중 기절해서 몽타주가 중단, 공격 속도 버프때문에 몽타주 재생속도가 변경됨, 다른 공격 몽타주로 전환됨
같은 돌발상황에 대처가능

BlendOut이 있기전에 호출되는 델리게이트도 있는데 구분해서 사용하면 된다.

- Montage_SetBlendingOutDelegate() : 공격 자세에서 기본자세로 전환하기 시작할때 호출
- Montage_SetEndDelegate() : 전환까지 끝나서 몽타주의 영향력이 완전히 사라졌을때 호출

BlendingOutDelegate는 주로 몽타주가 종료 전환을 시작하는 시점에 후처리가 필요할 때

