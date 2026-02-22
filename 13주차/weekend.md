# 팀프로젝트

## 애니메이션 수정

- 특정 애니메이션 실행시 발목이 돌아가는 현상 고침

# 줌 기능 해제분석

```c++
UAbilitySystemComponent* MyASC = GetAbilitySystemComponentFromActorInfo();
    if (MyASC && ZoomEffectClass)
    {
       // Effect Context 세팅 및 Spec 생성
       FGameplayEffectContextHandle EffectContext = MyASC->MakeEffectContext();
       EffectContext.AddSourceObject(GetAvatarActorFromActorInfo());

       FGameplayEffectSpecHandle SpecHandle = MyASC->MakeOutgoingSpec(ZoomEffectClass, 1.0f, EffectContext);
       if (SpecHandle.IsValid())
       {
          ActiveZoomEffectHandle = MyASC->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
       }
    }
```

- MakeEffectContext
  - GE의 배경정보를 담은 Context
  - 누가 어떤 데미지를 어디에다가 줬는지?
  - 리턴값은 컨텍스트 데이터를 가라키는 포인터
- MakeOutgoingSpec
  - 구체적으로 작성
  - (적용할 GE클래스, 레벨, Context)
  - GE클래스는 그냥 틀일 뿐이고 실제로 동적 데이터 구조체로 인스턴스화 함 ( 실제로 적용 하기위한 계산 )

- ApplyGameplayEffectSpecToSelf
  - 명세서(Spec) 제출 및 실제 적용
  - 실제로 ASC에 제출해서 이펙트를 발생시킨다.
  - \*SpecHandle.Data.Get() : 명세서의 실제 데이터 참조

```c++
// 줌은 버튼을 떼면 종료되므로 WaitInputRelease 사용
UAbilityTask_WaitInputRelease* WaitReleaseTask = UAbilityTask_WaitInputRelease::WaitInputRelease(this);

if (WaitReleaseTask)
{
       // 신호가 오면 어빌리티 종료 함수 연결
   WaitReleaseTask->OnRelease.AddDynamic(this, &UGA_Zoom::OnInputReleased);
   WaitReleaseTask->ReadyForActivation();
}

void UGA_Zoom::OnInputReleased(float TimeHeld)
{
    // 입력이 끝나면 어빌리티 종료
    EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}

void UGA_Zoom::EndAbility(
    const FGameplayAbilitySpecHandle Handle,
    const FGameplayAbilityActorInfo* ActorInfo,
    const FGameplayAbilityActivationInfo ActivationInfo,
    bool bReplicateEndAbility,
    bool bWasCancelled)
{
    // 어빌리티 종료 시 GE(줌 상태 관련 효과 등) 제거
    if (ActiveZoomEffectHandle.IsValid())
    {
       GetAbilitySystemComponentFromActorInfo()->RemoveActiveGameplayEffect(ActiveZoomEffectHandle);
       ActiveZoomEffectHandle.Invalidate();
    }

    Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}
```

- 줌떼면 OnInputReleased를 호출
- 적용했던 이펙트를 EndAbility에서 제거
- GetAbilitySystemComponentFromActorInfo()->RemoveActiveGameplayEffect(ActiveZoomEffectHandle)
  - 캐릭터에 달아놓은 ASC에서 ActiveZoomEffectHandle 이펙트를 찾아서 효과를 제거하고 캐릭터에서 떼어냄
- ActiveZoomEffectHandle.Invalidate()
  - 이 핸들을 Invalidate하게 만듬

```c++
void APlayerCharacter::InitializeAbilitySystem()
{
	if (AbilitySystemComponent)
	{
		AbilitySystemComponent->InitAbilityActorInfo(this, this);

	    // Zoom 태그용 델리게이트 바인딩
	    FGameplayTag ZoomTag = FGameplayTag::RequestGameplayTag(FName("State.Player.IsZooming"));

	    AbilitySystemComponent->RegisterGameplayTagEvent(
            ZoomTag,
            EGameplayTagEventType::NewOrRemoved
        ).AddUObject(this, &APlayerCharacter::OnZoomTagChanged);
	}
}
```

- RegisterGameplayTagEvent를 통해서 Zoom

```c++
void APlayerCharacter::OnZoomTagChanged(const FGameplayTag CallbackTag, int32 NewCount)
{
    // NewCount는 이 줌 태그를 몇개 가지고 있는가 숫자, 즉 1이면 줌, 0이면 줌 아닌상태
    bIsZooming = (NewCount > 0);
}
```

- GAS는 태그를 몇개 가지고 있는지로 판단한다.

```c++
void APlayerCharacter::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    if (FollowCamera)
    {
        float TargetFOV = bIsZooming ? 45.0f : 90.0f;
        float CurrentFOV = FollowCamera->FieldOfView;

        if (!FMath::IsNearlyEqual(CurrentFOV, TargetFOV, 0.1f))
        {
            float NewFOV = FMath::FInterpTo(CurrentFOV, TargetFOV, DeltaTime, 10.0f);
            FollowCamera->SetFieldOfView(NewFOV);
        }
        else if (CurrentFOV != TargetFOV)
        {
            FollowCamera->SetFieldOfView(TargetFOV);
        }
    }
}
```

- Tick함수를 사용해서 카메라 FOV(Field of view)를 수정한다.
- 줌이면 45도, 노줌이면 90도
- 목표 FOV와 차이가 별로 안나면 서서히 바뀌게 끔

![](https://velog.velcdn.com/images/kyu_/post/e242f7f2-d062-4a25-a230-1774890df1fd/image.png)

- 에디터에서 Sprint Arm의 Use Pawn Control Rotation을 true로해서 카메라 시점을 자유롭게 변경

## 결과

<iframe width="560" height="315"
src="https://www.youtube.com/embed/UaYWp8-Edec"
title="YouTube video player" frameborder="0"
allow="accelerometer; autoplay; clipboard-write; encrypted-media; gyroscope; picture-in-picture"
allowfullscreen></iframe>
