# 팀프로젝트

## 프로젝트 해체분석

### CommitAbility

- 스킬을 쓰기 위한 최종 승인 절차
- 조건이 맞으면 실제로 자원(스태미너)을 갂고 쿨타임을 돌리는 실행 기능까지 포함
  - Check : 조건에 맞는지 검사
  - Apply : 검사를 통과하면 즉시 스태미너 차감 및 쿨타임 적용

### Ability Task

#### PlayMontageAndWait

- 애니메이션의 시작과 끝을 관리

```c++
UAbilityTask_PlayMontageAndWait* PlayMontageTask =
UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(
    this, // 어빌리티 자기 자신
    NAME_None, // Task 별명
    RollMontage, // 실제 재생할 Montage
    1.0f // 재생 속도
);
```

- 애니메이션을 옆에서 감시하는 주체
- CreatePlayMontageAndWaitProxy : 태스크 객체 (감시 주체) 생성

```c++
// Montage 콜백 연결
PlayMontageTask->OnCompleted.AddDynamic(this, &UGA_Roll::OnMontageEnded);
PlayMontageTask->OnCancelled.AddDynamic(this, &UGA_Roll::OnMontageEnded);
PlayMontageTask->OnInterrupted.AddDynamic(this, &UGA_Roll::OnMontageEnded);
// Task 활성화
PlayMontageTask->ReadyForActivation();
```

- 위 코드처럼 델리게이트 바인딩을 해놓음
  - OnCompleted, OnCancelled, OnInterrupted 상황이 발생했을때 OnMontageEnded를 호출(알려줘라)

- ReadyForActivation() : 얘를 호출해야 애니메이션이 재생하고 감시를 함

#### WaitGameplayEvent

- 얘는 좀 다름
- 어떤 애니메이션이 실행될때 그 도중에 발생하는 특정 시점을 잡아내기 위해 사용함

```c++
UAbilityTask_WaitGameplayEvent* WaitEventTask =
    UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(
        this,
        FGameplayTag::RequestGameplayTag(FName("Event.Montage.Roll"))
    );

if (WaitEventTask)
{
    WaitEventTask->EventReceived.AddDynamic(this, &UGA_Roll::OnRollGameplayEvent);
    WaitEventTask->ReadyForActivation();
}

// 공격들이 회피가 된다
void UGA_Roll::OnRollGameplayEvent(FGameplayEventData EventData)
{
	APlayerCharacter* Character = Cast<APlayerCharacter>(GetAvatarActorFromActorInfo());
	UAbilitySystemComponent* MyASC = GetAbilitySystemComponentFromActorInfo();

	if (!Character || !MyASC) return;

	// 컨텍스트 생성, 효과가 어디서부터 나타났는지, 추후에 데미지 계산이나 로그 시스템에서 누가 사용한지 알 수 있음
	FGameplayEffectContextHandle EffectContext = GetAbilitySystemComponentFromActorInfo()->MakeEffectContext();
	EffectContext.AddSourceObject(Character);

	// 실제로 적용될 효과를 작성, GE클래스
	FGameplayEffectSpecHandle SpecHandle = GetAbilitySystemComponentFromActorInfo()->MakeOutgoingSpec(
		RollEffectClass,
		1.0f, // Level
		EffectContext
	);

	if (SpecHandle.IsValid())
	{
		// 나에게 발생하므로 나한테 효과 적용
		ActiveRollEffectHandle = MyASC->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
	}
}
```

- 내 코드를 예시로 들자면 구르기를 할때 OnRollGameplayEvent를 연결해놓았음
- Event.Montage.Roll이라는 태그를 가진 이벤트가 감지되면 OnRollGameplayEvent가 발생
- 이는 구를때 무적상태를 만들어주기위해 이렇게 만들었음
- 그래서 함수안에는 GE를 부르는 로직을 담아놓음

### Sprint의 경우 왜 PlayMontageAndWait을 안썼을까??

```c++
void UGA_Sprint::ActivateAbility(
	const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData)
{
	// 스킬 사용을 위한 조건 체크 , 쿨타임, 코스트다 있는지
	if (!CommitAbility(Handle, ActorInfo, ActivationInfo))
	{
		// Commit 실패 (스태미너 부족, Cooldown 중 등)
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return;
	}

	UAbilitySystemComponent* MyASC = GetAbilitySystemComponentFromActorInfo();
	if (MyASC && SprintEffectClass)
	{
		FGameplayEffectContextHandle EffectContext = MyASC->MakeEffectContext();
		EffectContext.AddSourceObject(GetAvatarActorFromActorInfo());

		FGameplayEffectSpecHandle SpecHandle = MyASC->MakeOutgoingSpec(SprintEffectClass, 1.0f, EffectContext);
		if (SpecHandle.IsValid())
		{
			ActiveSprintEffectHandle = MyASC->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
		}
	}

	UAbilityTask_WaitGameplayEvent* WaitStopTask = UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(
		this,
		FGameplayTag::RequestGameplayTag(FName("Event.Montage.Sprint"))
	);

	if (WaitStopTask)
	{
		// 신호가 오면 어빌리티 종료 함수 연결
		WaitStopTask->EventReceived.AddDynamic(this, &UGA_Sprint::OnSprintStopReceived);
		WaitStopTask->ReadyForActivation();
	}
}

void UGA_Sprint::OnSprintStopReceived(FGameplayEventData Payload)
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}
```

- Sprint의 ActivateAbility
- 보면 즉시 GameplayEffect를 적용시킨다.
- 또, WaitGameplayEvent의 역할도 Event.Montage.Sprint태그가 들어오면 바로 종료를 시키는 역할밖에 안한다.
- 구르기와 차이가 무엇일까????

- 구르기는 한번 눌렀을때 애니메이션이 무조건 끝까지 재생된다.
- 달리기는 구르기와달리 Shift를 눌렀을때만 달리는 동작을 유지하고 떼면 동작이 끝난다.
- PlayMontageAndWait을 쓰면 달리기 애니메이션이 끝날때까지 재생된다. 그래서 좀 어색하다.

## 카메라 위치

![](https://velog.velcdn.com/images/kyu_/post/26a50b08-bae4-41c2-8c1f-2e1b93ab8fe7/image.png)

![](https://velog.velcdn.com/images/kyu_/post/e0108ef6-c6c2-4c45-b728-aae4fbb3c875/image.png)

- 기존 카메라 위치 및 Location수치

### 문제점

![](https://velog.velcdn.com/images/kyu_/post/4f2f2b2b-13a0-4fbc-af98-ba2955add6e2/image.png)

![](https://velog.velcdn.com/images/kyu_/post/3c8715e6-032c-4b77-80a9-99babfbc7a9b/image.png)

- Do Collision Test가 체크되어있는데도 이런식으로 벽을 통과해서 보임

### 해결방법

- Socket Offset : 시작점은 놔두고 카메라 위치만 변경한다.
- 카메라의 Location을 변경하지 않고 Offset을 변경해서 해결

- Location은 다시 0, 0, 70으로

![](https://velog.velcdn.com/images/kyu_/post/efd945c5-fb79-4110-8a0d-d9d490bf4085/image.png)

- Socket Offset을 0, 50, 0으로

![](https://velog.velcdn.com/images/kyu_/post/0518b74c-301e-4bed-bc10-e85cf7643711/image.png)

- 캐릭터 뷰포트

![](https://velog.velcdn.com/images/kyu_/post/5d927ba2-76a7-4d35-9f07-312c0fff82f9/image.png)

- 결과

![](https://velog.velcdn.com/images/kyu_/post/b88db1de-7d3e-4901-8a29-fa05eb70b01d/image.png)
