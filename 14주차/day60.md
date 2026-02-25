# 개념 공부

- Tag와 Payload

https://velog.io/@kyu_/GAS-%EC%96%B4%EB%B9%8C%EB%A6%AC%ED%8B%B0-%ED%83%9C%EA%B7%B8-%EC%A0%9C%EC%96%B4


# 팀프로젝트

# Death 기능

- 저번시간에 피격로직을 만들었었다.
- 오늘은 피가 0으로 떨어지면 죽는 모션을 추가해보려고 한다.

## AttributeSet 수정

- 우선 데미지로직에 피가 0으로 떨어질때 죽음 GA를 호출할 수 있게끔 함수를 만들어주었다.

```c++
// 들어온 데미지 저장
const float DamageValue = GetDamage();
SetDamage(0.f);

// 데미지가 0이하면 무시
if (DamageValue <= 0.f) return;

// 피 보정
const float NewHealth = FMath::Clamp(GetHealth() - DamageValue, 0.0f, GetMaxHealth());
SetHealth(NewHealth);

// 데미지를 받고서 피가 0이상이면 피격효과 
HandleHitReaction(DamageValue);

// 피가 0 이하면 사망로직
if (NewHealth <= 0) HandleDeath();
```

- 기존 GameplayCue를 사용하는 피격효과는 함수 HandleHitReaction로 분리했다.
- 피가 0이하면 HandleDeath함수에서 로직을 수행한다.

### HandleDeath

```c++
void UCharacterAttributeSet::HandleDeath()
{
    UAbilitySystemComponent* ASC = GetOwningAbilitySystemComponent();
    if (!ASC) return;
    FGameplayTagContainer AbilityTags;
    AbilityTags.AddTag(FGameplayTag::RequestGameplayTag(FName("Ability.Player.Death")));
    ASC->TryActivateAbilitiesByTag(AbilityTags);
}
```
- 우선 GetOwningAbilitySystemComponent를 통해 ASC를 가져온다.
- Ability.Player.Death를 가진 GA를 찾아서 실행시켜준다.


## GA_Death

- 다른 GA함수들과 큰차이는 없고 생성자에서 Ability로 시작하는 태그를 가진 GA들은 모두 취소하게 만들어 주었다.
  - 죽을때 이동하거나 앉거나 이러면 안되므로
  - 태그의 계층구조가 이때 편하다고 느꼈다.

```c++
UGA_Death::UGA_Death()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
	
	AbilityTags.AddTag(FGameplayTag::RequestGameplayTag(FName("Ability.Player.Death")));
	
	CancelAbilitiesWithTag.AddTag(FGameplayTag::RequestGameplayTag(FName("Ability")));
	ActivationOwnedTags.AddTag(FGameplayTag::RequestGameplayTag(FName("State.Player.Death")));
}
```

- 아래쪽은 기존 GA와 비슷하게 PlayMontageAndWait으로 Montage를 실행해주었다.


## 에디터 설정

### 죽는 모션 리타겟팅

- 죽는모션을 믹사모에서 가져와서 리타겟팅 해주었다.

### BF_GE_Death 설정

- GE는 C++에서 구현하는게 없으므로 블루프린트 클래스로 GameplayEffect를 상속받아서 만들었다.

![](https://velog.velcdn.com/images/kyu_/post/b0274b50-0e2a-44b3-8b88-e3258114374f/image.png)

- Duration은 Infinite로 설정
- Grant Tag를 State.Player.Death로 설정해서 이 GE_Death를 실행중에 캐릭터에게 State.Player.Death 즉 죽었다는 태그를 달아주기 위함
- 혹시 몰라서 Health도 override로 0으로 설정해주었다.


### BP_GA_Death 설정

- 리타겟팅 해온 Death Montage와 Death Effect Class에 BP_GE_Death를 넣어주었다.
- 이제 G키를 연타해서 캐릭터를 죽여보자

# 결과

<iframe width="560" height="315"
src="https://www.youtube.com/embed/dhjtx8DHyww"
title="YouTube video player" frameborder="0"
allow="accelerometer; autoplay; clipboard-write; encrypted-media; gyroscope; picture-in-picture"
allowfullscreen></iframe>

- 우선 동작은 잘되는데 3가지 문제점이 있다.

1. 캐릭터가 죽은뒤에 움직일 수 있다.
2. 앉은상태에서도 일어나서 죽는 모션이 어색하다.
3. 엎드리기도 마찬가지


## 캐릭터가 움직이는 문제


```c++
void APlayerCharacter::Move(const FInputActionValue& value)
{
    if (!AbilitySystemComponent) return;
    if (!Controller) return;
    
    // 장전 중, 엎드린상태에서 공격중에는 움직일 수 없음
    FGameplayTag ReloadingTag = FGameplayTag::RequestGameplayTag(FName("State.Player.IsReloading"));
    FGameplayTag AttackingTag = FGameplayTag::RequestGameplayTag(FName("State.Player.IsAttacking"));
    FGameplayTag ProneTag = FGameplayTag::RequestGameplayTag(FName("State.Player.IsProning"));
    FGameplayTag DeathTag = FGameplayTag::RequestGameplayTag(FName("State.Player.Death"));

    const bool bIsReloading = AbilitySystemComponent->HasMatchingGameplayTag(ReloadingTag);
    const bool bIsAttacking = AbilitySystemComponent->HasMatchingGameplayTag(AttackingTag);
    const bool bIsProning   = AbilitySystemComponent->HasMatchingGameplayTag(ProneTag);
    const bool bIsDeath   = AbilitySystemComponent->HasMatchingGameplayTag(DeathTag);
    
    if (bIsReloading || (bIsAttacking && bIsProning) || bIsDeath) return;
```

- 캐릭터 이동 함수에서 DeathTag를 만들고
- Death태그를 ASC가 가지면 bIsDeath라는 bool함수를 true로 만들어서 죽을때는 움직이지 못하게 만들었다.

<iframe width="560" height="315"
src="https://www.youtube.com/embed/y73axJGlE2I"
title="YouTube video player" frameborder="0"
allow="accelerometer; autoplay; clipboard-write; encrypted-media; gyroscope; picture-in-picture"
allowfullscreen></iframe>


## 앉은상태, 엎드린상태일때 모션 추가

- 일전에 공부했던 PayLoad를 사용할때

### AttributeSet

```c++
void UCharacterAttributeSet::HandleDeath()
{
    UAbilitySystemComponent* ASC = GetOwningAbilitySystemComponent();
    if (!ASC) return;
    FGameplayEventData Payload;
    Payload.EventTag = FGameplayTag::RequestGameplayTag(FName("Ability.Player.Death"));
    
    if (ASC->HasMatchingGameplayTag(FGameplayTag::RequestGameplayTag(FName("State.Player.IsCrouching"))))
    {
        Payload.EventMagnitude = 1.0f;
    }
    else if (ASC->HasMatchingGameplayTag(FGameplayTag::RequestGameplayTag(FName("State.Player.IsProning"))))
    {
        Payload.EventMagnitude = 2.0f;
    }
    else
    {
        Payload.EventMagnitude = 0.0f;
    }
    ASC->HandleGameplayEvent(Payload.EventTag, &Payload);
}
```

- AttributeSet의 HandleDeath함수를 이런식으로 바꿔주었다.
- Payload와 함께 캐릭터의 현재 상태를 EventMagnitude로 치환해서 같이 보내주었다.
  - 1은 앉기, 2는 엎드리기, 기본은 0

### GA_Death

```c++
void UGA_Death::ActivateAbility(
	const FGameplayAbilitySpecHandle Handle, 
	const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo, 
	const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	
	if (!TriggerEventData)
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return;
	}
	
	float StateValue = TriggerEventData->EventMagnitude;
	UAnimMontage* SelectedMontage = DeathMontage;
	
	if (StateValue == 1.0f)
	{
    	SelectedMontage = CrouchDeathMontage;
	}
	else if (StateValue == 2.0f)
	{
		SelectedMontage = ProneDeathMontage;
	}

	UAbilityTask_PlayMontageAndWait* PlayMontageTask = 
		UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(
		this,
		NAME_None,
		SelectedMontage,
		1.0f
		);
	if (!PlayMontageTask) return;

	PlayMontageTask->OnCompleted.AddDynamic(this, &UGA_Death::OnMontageEnded);
	PlayMontageTask->OnInterrupted.AddDynamic(this, &UGA_Death::OnMontageEnded);
	PlayMontageTask->OnCancelled.AddDynamic(this, &UGA_Death::OnMontageEnded);
	PlayMontageTask->ReadyForActivation();
}
```

- ActivateAbility에서 TriggerEventData로 받아주어서 StateValue에 따라서 다른 동작을 하게끔 구현했다.

<iframe width="560" height="315"
src="https://www.youtube.com/embed/NByF77JbGXU"
title="YouTube video player" frameborder="0"
allow="accelerometer; autoplay; clipboard-write; encrypted-media; gyroscope; picture-in-picture"
allowfullscreen></iframe>

- 이제는 서있어도 죽지를 않는다.

### BP_GA_Death 설정

- 해결방법은 생각보다 쉬운거였다.

![](https://velog.velcdn.com/images/kyu_/post/d8745b93-7fdb-44c0-90ca-9a674443895a/image.png)

- Ability Triggers에서 Trigger Tag를 우리가 Payload호출할때 부르는 태그로 설정하고 Trigger Source를 Gameplay Event로변경하면 정상적으로 작동하였다.

#### Ability Trigger

- ASC가 이벤트를 받았을때 어떤 어빌리티를 깨울것인지 결정하는 라우팅 테이블 역할
- ASC는 여러 어빌리티를 가지고 있는데 외부에서 HandleGameplayEvent라는 신호가 왔을때 누가 처리해야할지 결정해야함
- TryActivateAbilitiesByTag만 사용하는 기존에는 안써줘도 상관없지만 데이터와 함께 넘어오는 HandleGameplayEvent를 사용할때는 꼭 지정해주어야 동작하더라

- 그리고 기존에 페이로드를 부를때 Ability.Player.Death 어빌리티 자체 태그를 사용했는데 이것은 사망어빌리티 자체의 정체성같은 태그라서 이럴때는 사용하지 않는것이 좋다고 한다.
- Event태그로 사용하는것이 낫다고 해서 Event.Montage.Death로 기존 다른 Event 태그와 맞춰서 변경하였다.

<iframe width="560" height="315"
src="https://www.youtube.com/embed/jjIf4mopeE8"
title="YouTube video player" frameborder="0"
allow="accelerometer; autoplay; clipboard-write; encrypted-media; gyroscope; picture-in-picture"
allowfullscreen></iframe>

## 태그안가지고 있는 버그

- GE_Death에서 State.Player.Death 태그를 Infinite로 가지고 있게 설정했는데 실제로는 없었다.

### GA_Death

```c++
if (!DeathEffectClass)
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return;
	}
	
	// 추후에 누가 죽였는지 필요하면 작성하고 우선은 빼두자
	// FGameplayEffectContextHandle EffectContext = MakeEffectContext(Handle, ActorInfo);
	FGameplayEffectSpecHandle SpecHandle = MakeOutgoingGameplayEffectSpec(DeathEffectClass, GetAbilityLevel());
	if (SpecHandle.IsValid())
	{
		ApplyGameplayEffectSpecToOwner(Handle, ActorInfo, ActivationInfo, SpecHandle);
	}
```

- GA_Death의 DeathEffectClass에 BP_GE_Death를 지정만하고 할당해주지 않았다.
- 정상적으로 태그를 가지고 있게 되었다.

## 죽었는데 앉기, 엎드리기 가능한 버그

- Payload를 통해서 엎드리기 앉기에 대한 유효성은 빡빡하게 검사하지 않아서 발생한 버그인것 같다.

<iframe width="560" height="315"
src="https://www.youtube.com/embed/Qaew9GUoyRY"
title="YouTube video player" frameborder="0"
allow="accelerometer; autoplay; clipboard-write; encrypted-media; gyroscope; picture-in-picture"
allowfullscreen></iframe>

### 해결방법

- GA_Prone, GA_Crouch의 생성자에서 Block태그에 Death를 넣어주었다.

```c++
ActivationBlockedTags.AddTag(FGameplayTag::RequestGameplayTag(FName("State.Player.Death")));
```

<iframe width="560" height="315"
src="https://www.youtube.com/embed/vz2b6XDyN6o"
title="YouTube video player" frameborder="0"
allow="accelerometer; autoplay; clipboard-write; encrypted-media; gyroscope; picture-in-picture"
allowfullscreen></iframe>

- 안보이지만 앉기, 엎드리기 키 연타중이다.

# 갖가지 수정

## PlayerCharacter 수정

- 앉기, 엎드리기시 점프되던 버그 수정
- Tag자주 불러오던 Move, Jump수정
- CanMove, CanJump 추가
  - 점프가능한지, 이동가능한지 판단해주는 함수
- static으로 TagContainer 만들어서 Move, Jump요청이 여러번 들어와도 캐싱되어있는 태그로 매칭해서 점프가능한지, 움직일수있는지 판단

## Sprint수정

- 앉아있고, 엎드려있을때 Sprint키 누르면 스태미너 달던 버그 수정

## GA수정

- GA의 AcitvateAbility에서 유효성검사 수정
  - 그냥 return해주던 부분에 EndAbility추가 
  - EndAbility를 추가하지 않으면 어빌리티가 끝나지 않고 루프될수도 있다.
  
