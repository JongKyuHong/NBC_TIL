# 총기 반동 만들기

- 총기 반동을 만들어보자
- 캐릭터가 반동정보에 대해서 알필요가없으므로 PlayerCharacter에다가 만들지않고 CombatComponent안에다가 넣기로 하였다.
- 대략적인 흐름은 이렇다

1. GA_Attack에서 공격 몽타주를 실행할때쯤에 CombatComponent의 반동계산로직함수를 호출
2. CombatComponent의 ApplyRecoil함수에서 반동계산
3. Tick함수에서 반동정보??를 보간을 사용해서 부드럽게 적용

## GA_Attack

```c++
// 발사 실행
if (HasAuthority(&CurrentActivationInfo))
{
	ApplyRecoil();
	PlayRecoilMontage();
	SpawnProjectile();
	GenerateFiringNoise();
}

void UGA_Attack::ApplyRecoil()
{
    APlayerCharacter* Player = Cast<APlayerCharacter>(GetAvatarActorFromActorInfo());
    if (!Player) return;
    UCombatComponent* CombatComponent = Player->GetCombatComponent();
    if (!CombatComponent) return;
    CombatComponent->ApplyRecoil();
}
```

- 발사 실행하는 부분에서 ApplyRecoil을 호출
- ApplyRecoil에서 캐릭터의 정보를 통해서 캐릭터에 붙어있는 CombatComponent정보를 가져옴
- CombatComponent의 ApplyRecoil함수 호출

## CombatComponent 헤더

```c++
virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

/** 반동값 저장 벡터 변수*/
FVector2D TargetRecoil;
FVector2D CurrentRecoil;

/** 마지막으로 사격한 시간*/
float LastFireTime = 0.0f;

/** 반동 회복 시간 */
UPROPERTY(EditAnywhere, Category = "Recoil")
float RecoilRecoveryDelay = 0.1f;

/** 반동에 의해 총이 흔들리는 속도*/
UPROPERTY(EditAnywhere, Category = "Recoil")
float RecoilInterpSpeed = 15.0f;

/** 총기가 원점으로 복귀하는 속도*/
UPROPERTY(EditAnywhere, Category = "Recoil")
float RecoveryInterpSpeed = 10.0f;

/** 반동 계산 */
void ApplyRecoil();
```

- 컴포넌트도 Tick함수가 있을까해서 찾아보니 TickComponent를 사용하면 된다고 한다.

## CombatComponent.cpp

### ApplyRecoil함수

```c++
void UCombatComponent::ApplyRecoil()
{
    if (!ActiveWeapon) return;

    const FWeaponData& WeaponData = ActiveWeapon->GetCurrentDataAsset()->WeaponData;
    float RecoilAmount = WeaponData.DefaultRecoilValue;

    TargetRecoil.X += -RecoilAmount;

    float RandomYaw = FMath::RandRange(-RecoilAmount * 0.2f, RecoilAmount * 0.2f);
    TargetRecoil.Y += RandomYaw;

    LastFireTime = GetWorld()->GetTimeSeconds();
}
```

- 총기마다 각자 반동값이 데이터에셋으로 저장되어있는데 이를 불러와서 계산을 해준다.
- 좌우의 경우 너무 많이 흔들려서 보정을 해주고 랜덤값을 주었다.

### TickComponent함수

```c++
void UCombatComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

    /** 현재시간 구하기 */
    float CurrentTime = GetWorld()->GetTimeSeconds();
    /** 현재시간에서 총쏜시간을 뺀값이 딜레이보다 크면 (사격 후 일정시간이 지나면)
     * 목표반동값을 0,0 으로 설정한다.
     */
    if (CurrentTime - LastFireTime > RecoilRecoveryDelay)
    {
        TargetRecoil = FMath::Vector2DInterpTo(TargetRecoil, FVector2D::ZeroVector, DeltaTime, RecoveryInterpSpeed);
    }
    /** 목표 반동을 향해서 부드럽게 반동*/
    FVector2D NewRecoil = FMath::Vector2DInterpTo(CurrentRecoil, TargetRecoil, DeltaTime, RecoilInterpSpeed);

    /** 이번 프레임에서 움직일 변화량을 저장*/
    FVector2D RecoilDelta = NewRecoil - CurrentRecoil;
    CurrentRecoil = NewRecoil;

    APawn* OwnerPawn = Cast<APawn>(GetOwner());
    if (!OwnerPawn) return;
    APlayerController* PlayerController = Cast<APlayerController>(OwnerPawn->GetController());
    if (!PlayerController) return;

    /** 위에서 구한 변화량만큼 변화*/
    PlayerController->AddPitchInput(RecoilDelta.X);
    PlayerController->AddYawInput(RecoilDelta.Y);
}
```

- 틱에서 실제로 적용해준다.
- Controller의 AddPitchInput, AddYawInput으로 반동을 구현
- Interp덕에 순간이동하지않고 부드럽게 이동

# 결과

<iframe width="560" height="315"
src="https://www.youtube.com/embed/0s4qd6LSVLE"
title="YouTube video player" frameborder="0"
allow="accelerometer; autoplay; clipboard-write; encrypted-media; gyroscope; picture-in-picture"
allowfullscreen></iframe>

# 피스톨들고 이동 개선

## 개선 전

<iframe width="560" height="315"
src="https://www.youtube.com/embed/sw52wK_SdFA"
title="YouTube video player" frameborder="0"
allow="accelerometer; autoplay; clipboard-write; encrypted-media; gyroscope; picture-in-picture"
allowfullscreen></iframe>

- 피스톨들고 이동시 애니메이션이 굉장히 어색하다.
- 특히 왼쪽 대각 앞, 왼쪽 대각 뒤, 왼쪽 이동이 전체적으로 어색하고 땅에 발이 들어가는 등 여러가지 문제가 있었다.

## 발생 원인

- 애니메이션을 믹사모에서 찾았는데 라이플을 들고있는 애니메이션은 대각이동이나 방향별 걷기, 뛰기가 있었다.
- 피스톨의 경우에는 그렇지않고 대각이동이 없고, 걷고 뛰는 모션도 방향별로 다 존재하지는 않았다

## 시도 방법

- 정말 많은 고민을 하고 여러가지를 시도했었다.

### 다른 애니메이션 찾아보기

- 권총을 들고있는 마땅한 애니메이션을 찾기가 어려웠다.
- 믹사모말고 fab에서도 여러가지를 찾아서 시도해보았지만 마땅하지 않았다.

### 포즈 워핑

https://dev.epicgames.com/documentation/ko-kr/unreal-engine/pose-warping-in-unreal-engine

- 공식문서를 읽고 굉장히 가능성이 높아서 도입해보았는데 도입 실패하였다.
- IK Reg쪽에서 문제가 발생했는데 정확한 원인은 시간문제상 알아내지못했고 폐기했다.

## 해결 방법

### Layered blend per bone

- 이전에 상하체 분리를 위해서 사용했던 노드
- 라이플의 애니메이션은 이동이 괜찮았기때문에 하체는 라이플의 애니메이션을 사용하고 상체는 피스톨 애니메이션을 이용하기로 하였다.
- 현실적으로 시간이 부족하기때문에 찾은 방법

![](https://velog.velcdn.com/images/kyu_/post/38ccdcf3-266d-4114-9c33-9dcbae4eaee0/image.png)

![](https://velog.velcdn.com/images/kyu_/post/e6bc5411-d778-4907-b66e-6ffc7b8784e0/image.png)

- Mesh Space Rotation Blend 체크
  - 활성화 시 상체 애니메이션의 회전값이 하체의 움직임과 상관없이 캐릭터 메쉬의 정면을 기준으로 유지

# 개선 결과

<iframe width="560" height="315"
src="https://www.youtube.com/embed/lk7FN_wdAsQ"
title="YouTube video player" frameborder="0"
allow="accelerometer; autoplay; clipboard-write; encrypted-media; gyroscope; picture-in-picture"
allowfullscreen></iframe>

- 이전보다는 맘에드는 결과를 얻었다.