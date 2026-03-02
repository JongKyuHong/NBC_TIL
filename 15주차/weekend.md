- 여태 공부하고 사용해봤던 GA, GE, GCN을 활용해서 특수능력을 만들어보자

# GA

- 다른 GA들과 비슷했다. ActivateAbility, EndAbility를 오버라이드하고,
- 시간을 느리게 가고싶어서 시간계수랑 변화적용시키는 함수를 만들었다.
- 나머지는 실제 적용될 cue, ge를 정의하는 식으로 갔다.

```c++
/** 시간 계수 */
UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Time")
float SlowRate;

/** 사람할테 적용될 배수 */
UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Time")
float SlowRateForPlayer;

/** 적용할 CueTag*/
UPROPERTY(EditAnywhere, Category = "GAS | Cue")
FGameplayTag SpecialAbilityCueTag;

/** 지속적으로 게이지를 소모할 GameplayEffect */
UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cost")
TSubclassOf<UGameplayEffect> StaminaDrainGEClass;

FActiveGameplayEffectHandle StaminaDrainHandle;

/** 스태미너 변경시 호출*/
void OnStaminaChanged(const FOnAttributeChangeData& Data);

FDelegateHandle StaminaChangeDelegateHandle;

/** 시간 적용 함수*/
void ApplyTime(float GlobalFactor, float PlayerFactor);
```

## ApplyTime함수

```c++
// 호출
ApplyTime(SlowRate, SlowRateForPlayer);

void UGA_SpecialAbility::ApplyTime(float GlobalFactor, float PlayerFactor)
{
    if (UWorld* World = GetWorld())
    {
        World->GetWorldSettings()->SetTimeDilation(GlobalFactor);
    }

    if (AActor* Avatar = GetAvatarActorFromActorInfo())
    {
        Avatar->CustomTimeDilation = PlayerFactor;
    }
}

```

- World->GetWorldSettings()->SetTimeDilation(GlobalFactor) : 월드의 속도를 늦추고
- Avatar->CustomTimeDilation = PlayerFactor : 사용자는 보정이 들어감
- SlowRate의 기본값은 0.2로 세상은 5배느려지지만 SlowRateForPlayer에서 기본값을 2로설정해 캐릭터는 다른 사물들에 비해서 2배빠르게 움직이게 조정하였다.

# 에디터 설정

## 머티리얼 설정

![](https://velog.velcdn.com/images/kyu_/post/3bf9caf9-1fa0-46d5-af93-8861b25a9418/image.png)

- 특수능력중 화면을 주황빛으로 만들기위해서 적용

## GCN 블루프린트

![](https://velog.velcdn.com/images/kyu_/post/13507c42-9595-4950-94f6-bef6e1d4511b/image.png)

- 특수능력을 사용하면 소리와 화면이 서서히 켜지고 서서히 꺼지는것으로 설정
  - GCN에 오디오 Component를 달아주고 효과음을 넣어준 후 특수능력이 켜질때는 Play로, 특수능력이 꺼질때는 Fade Out으로 서서히 꺼지게끔 했다.
    - 소리 자체에 서서히 켜지는 효과가있어서 꺼질때만 서서히 꺼지게끔 설정

- 이 구간에서 가장많은 시간을 들였는데 오류 난것들을 안모아놓아서 결과만..

##

- 캐릭터에도 Default Abilities에 등록해주었다.
- IA_SA를 만들고 IMC에 달아주었다.

# 결과

<iframe width="560" height="315"
src="https://www.youtube.com/embed/-5g1kx2KZdU"
title="YouTube video player" frameborder="0"
allow="accelerometer; autoplay; clipboard-write; encrypted-media; gyroscope; picture-in-picture"
allowfullscreen></iframe>
