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
