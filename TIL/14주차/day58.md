
# 기능 구현

## 스태미너

![](https://velog.velcdn.com/images/kyu_/post/4272c5a7-e5a9-4294-b6c7-d79081c497be/image.png)

- 새로운 스태미너 GE를 만들었다.
- 이를 BP로 상속해서 각각 GE_StaminaDrain, ForRoll을 만들었다.

### StaminaDrain

- 달릴때 스태미너 감소
- 0.1초마다 -1

![](https://velog.velcdn.com/images/kyu_/post/582bd709-f1a7-4ed4-9eb1-3e151b8858a7/image.png)

![](https://velog.velcdn.com/images/kyu_/post/11f382cd-3d49-4f1c-9987-e969611bab50/image.png)


```c++
UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Sprint")
	TSubclassOf<UGameplayEffect> SprintStaminaEffectClass;
	
FGameplayEffectSpecHandle StaminaSpecHandle = MyASC->MakeOutgoingSpec(SprintStaminaEffectClass, 1.0f, EffectContext);
	if (StaminaSpecHandle.IsValid())
	{
		FGameplayTag CostTag = FGameplayTag::RequestGameplayTag(FName("Player.Stats.Stamina"));
		StaminaSpecHandle.Data.Get()->SetSetByCallerMagnitude(CostTag, -1.0f);
		
		ActiveSprintStaminaEffectHandle = MyASC->ApplyGameplayEffectSpecToSelf(*StaminaSpecHandle.Data.Get());
	}
```

- SprintStaminaEffectClass로 BP_GE_StaminaDrain을 등록하고 Sprint키를 누를때 Effect발생

```c++
StaminaChangeDelegateHandle = MyASC->GetGameplayAttributeValueChangeDelegate(
		   UCharacterAttributeSet::GetStaminaAttribute()
	   ).AddUObject(this, &UGA_Sprint::OnStaminaDepleted);

	UAbilityTask_WaitGameplayEvent* WaitStopTask = UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(
		this, 
		FGameplayTag::RequestGameplayTag(FName("Event.Montage.Sprint"))
	);

	if (!WaitStopTask) return;
	// 신호가 오면 어빌리티 종료 함수 연결
	WaitStopTask->EventReceived.AddDynamic(this, &UGA_Sprint::OnSprintStopReceived);
	WaitStopTask->ReadyForActivation();
```

- 변화 감지, 종료 감지

```c++
void APlayerCharacter::StopSprint(const FInputActionValue& Value)
{
    if (!AbilitySystemComponent) return;
    FGameplayEventData Payload;
    FGameplayTag StopTag = FGameplayTag::RequestGameplayTag(FName("Event.Montage.Sprint"));
    AbilitySystemComponent->HandleGameplayEvent(StopTag, &Payload);
}
```

- 캐릭터에서 Sprint의 ETriggerEvent::Completed에 매핑
- 매핑함수에서 Event.Montage.Sprint 태그를 통해 호출

```c++
void UGA_Sprint::OnStaminaDepleted(const FOnAttributeChangeData& Data)
{
	// 스태미나가 0 이하로 떨어졌다면 달리기를 강제 종료
	if (Data.NewValue <= 0.0f)
	{
		EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
	}
}

void UGA_Sprint::OnSprintStopReceived(FGameplayEventData Payload)
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}
```
- OnStaminaDepleted로 스태미너 변화 감지
  - 스태미너가 0이하로 깎이면 달리기 멈춤
- OnSprintStopReceived
  - 달리기를 멈추면 EndAbility 호출

```c++
void UGA_Sprint::EndAbility(
	const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo,
	bool bReplicateEndAbility,
	bool bWasCancelled)
{
	UAbilitySystemComponent* ASC = GetAbilitySystemComponentFromActorInfo();
	if (!ASC) return;
	
	// 어빌리티 종료시 GE(속도 증가 효과)제거
	if (ActiveSprintEffectHandle.IsValid())
	{
		ASC->RemoveActiveGameplayEffect(ActiveSprintEffectHandle);
		ActiveSprintEffectHandle.Invalidate();
	}

	if (ActiveSprintStaminaEffectHandle.IsValid())
	{
		ASC->RemoveActiveGameplayEffect(ActiveSprintStaminaEffectHandle);
		ActiveSprintStaminaEffectHandle.Invalidate();
	}

	if (StaminaChangeDelegateHandle.IsValid())
	{
		ASC->GetGameplayAttributeValueChangeDelegate(UCharacterAttributeSet::GetStaminaAttribute()).Remove(StaminaChangeDelegateHandle);
	}

	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}
```

- 여기서 다 정리

### StaminaDrainForRoll

- 구르기할때 스태미너 20감소

![](https://velog.velcdn.com/images/kyu_/post/95c937e1-c676-4ddb-b2f4-62511a08e7c0/image.png)

![](https://velog.velcdn.com/images/kyu_/post/ec6fa8b7-b51a-4faf-a23b-50d1a70d9db3/image.png)

![](https://velog.velcdn.com/images/kyu_/post/968a53e1-ec7a-445c-92c2-39567c9e9b9a/image.png)

- BP_GA_Roll의 Costs에 BP_GE_StaminaDrainForRoll등록해서 스태미너 감소하게끔 설정

## 상호작용

```c++
void APlayerCharacter::Interact(const FInputActionValue& Value)
{
    if (!AbilitySystemComponent) return;
    FGameplayTagContainer AbilityTags;
    AbilityTags.AddTag(FGameplayTag::RequestGameplayTag(FName("Ability.Player.Interact")));
    
    AbilitySystemComponent->TryActivateAbilitiesByTag(AbilityTags);
}
```

- 캐릭터에서 Interact 매핑 함수

```c++
void UGA_Interact::ActivateAbility(
    const FGameplayAbilitySpecHandle Handle, 
    const FGameplayAbilityActorInfo* ActorInfo, 
    const FGameplayAbilityActivationInfo ActivationInfo, 
    const FGameplayEventData* TriggerEventData)
{
    if (!CommitAbility(Handle, ActorInfo, ActivationInfo))
    {
        EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
        return;
    }
    
    APlayerController* PC = ActorInfo->PlayerController.Get();
    if (!PC)
    {
        EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
        return;
    }
    
    FVector CameraLocation;
    FRotator CameraRotation;
    PC->GetPlayerViewPoint(CameraLocation, CameraRotation);
    
    FVector StartTrace = CameraLocation;
    FVector EndTrace = StartTrace + (CameraRotation.Vector() * InteractDistance);
    
    FHitResult HitResult;
    
    FCollisionQueryParams QueryParams;
    QueryParams.AddIgnoredActor(ActorInfo->AvatarActor.Get());
    
    bool bHit = GetWorld()->LineTraceSingleByChannel(
        HitResult,
        StartTrace,
        EndTrace,
        ECC_Visibility,
        QueryParams
    );
    
    FColor DebugColor = bHit ? FColor::Green : FColor::Red;
    DrawDebugLine(GetWorld(), StartTrace, EndTrace, DebugColor, false, 2.0f, 0, 2.0f);
    
    if (bHit && HitResult.GetActor())
    {
        AActor* HitActor = HitResult.GetActor();
        
        // 상호작용 대상 로그 찍어보기
        UE_LOG(LogTemp, Warning, TEXT("상호작용 대상 이름: %s"), *HitActor->GetName());

        // 여기에 해당하는 아이템에 관한 상호작용 넣으면 될듯
    }
    
    EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
}
```

- PC-GetPlayerViewPoint(CameraLocation, CameraRotation)
  - 카메라 위치와 회전값, 우리는 크로스헤어에 있는 상호작용 물체와 상호작용 해야 하므로 이렇게 설정

- LineTraceSingleByChannel
  - ECC_Visibility라는 가시성 채널 사용 눈에 보이는 물체들이 다 걸림
  - QueryParams.AddIgnoreActor에 자기자신을 지정해서 자기자신은 걸리지 않도록 설정

- 상호작용하는 물체는 UE_LOG로 확인

# 결과

<iframe width="560" height="315"
src="https://www.youtube.com/embed/1778Z3SC1Kw"
title="YouTube video player" frameborder="0"
allow="accelerometer; autoplay; clipboard-write; encrypted-media; gyroscope; picture-in-picture"
allowfullscreen></iframe>


![](https://velog.velcdn.com/images/kyu_/post/e6ad4aec-c463-4d45-9f01-9051be0a5661/image.png)

# 튜터님 피드백

## 불필요한 중복 제거

- PlayerCharacter나 GA에서 불필요하게 ASC를 자주 불러오는 부분들을 수정했다.
- 특히 PlayerCharacter에서는 멤버변수로 ASC를 저장하는데 굳이 Get함수를 사용해서 가져오고 있었는데 그냥 멤버변수에 접근하는식으로 코드를 수정했다.

```c++
// 기존 코드
void APlayerCharacter::Move(const FInputActionValue& value)
{
    UAbilitySystemComponent* ASC = GetAbilitySystemComponent();
    if (!ASC) return;
    
    ...
}

// 변경

void APlayerCharacter::Move(const FInputActionValue& value)
{
    if (!AbilitySystemComponent) return;
```

- 일일히 GetAbilitySystemComponent를 호출해서 ASC를 얻는경우가 많았는데 다 수정했다.

## PlayerController 의존성 제거

```c++
// 기존 코드
void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	
	if (UEnhancedInputComponent* EnhancedInput = Cast<UEnhancedInputComponent>(PlayerInputComponent))
    {
        if (APlayerCharacterController* PlayerController = Cast<APlayerCharacterController>(GetController()))
        {
            if (PlayerController->MoveAction)
            {
                EnhancedInput->BindAction(PlayerController->MoveAction, ETriggerEvent::Triggered, this, &APlayerCharacter::Move);
            }

            if (PlayerController->LookAction)
            {
                EnhancedInput->BindAction(PlayerController->LookAction, ETriggerEvent::Triggered, this, &APlayerCharacter::Look);
            }

            if (PlayerController->JumpAction)
            {
                EnhancedInput->BindAction(PlayerController->JumpAction, ETriggerEvent::Started, this, &APlayerCharacter::StartJump);
                EnhancedInput->BindAction(PlayerController->JumpAction, ETriggerEvent::Completed, this, &APlayerCharacter::StopJump);
            }

// 수정 후
void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
    UEnhancedInputComponent* EnhancedInput = Cast<UEnhancedInputComponent>(PlayerInputComponent);
	if (!EnhancedInput) return;
    
    if (IA_Move)
    {
        EnhancedInput->BindAction(IA_Move, ETriggerEvent::Triggered, this, &APlayerCharacter::Move);
    }

    if (IA_Look)
    {
        EnhancedInput->BindAction(IA_Look, ETriggerEvent::Triggered, this, &APlayerCharacter::Look);
    }

    if (IA_Jump)
    {
        EnhancedInput->BindAction(IA_Jump, ETriggerEvent::Started, this, &APlayerCharacter::StartJump);
        EnhancedInput->BindAction(IA_Jump, ETriggerEvent::Completed, this, &APlayerCharacter::StopJump);
    }
```

- 기존에 PlayerController에 대한 캐릭터의 의존성을 제거하였다.
- PlayerController에 InputAction이 많이 선언되어있는 상태로 방치해두었었는데 다 삭제하고 캐릭터에서 일괄 관리하게끔 구조를 변경하였다. (Decoupling)


# 코드카타

## 교점에 별만들기

```c++
#include <string>
#include <vector>
#include <iostream>
#include <climits>
#include <algorithm>

using namespace std;

vector<string> solution(vector<vector<int>> line) {
    vector<pair<long long, long long>> vec;
    long long minX = LLONG_MAX;
    long long maxX = LLONG_MIN;
    long long minY = LLONG_MAX;
    long long maxY = LLONG_MIN;
    
    for (int i = 0; i < line.size(); i++){
        long long baseA = line[i][0];
        long long baseB = line[i][1];
        long long baseC = line[i][2];
        for (int j = i+1; j < line.size(); j++){
            long long targetA = line[j][0];
            long long targetB = line[j][1];
            long long targetC = line[j][2];
            
            long long tmp = baseA*targetB - targetA*baseB;
            if (tmp == 0) continue;
            long long valueX = baseB*targetC - targetB*baseC;
            long long valueY = targetA*baseC - baseA*targetC;
            long long x, y;
            // 정수쌍이 있으면
            if (valueX % tmp == 0 && valueY % tmp == 0){
                x = valueX/tmp;
                y = valueY/tmp;
                minX = min(x, minX);
                minY = min(y, minY);
                maxX = max(x, maxX);
                maxY = max(y, maxY);
                vec.push_back({x, y});
            }
        }
    }
    
    string row(maxX - minX + 1, '.');
    vector<string> answer(maxY - minY + 1, row);
    
    for (auto v : vec){
        int r = maxY - v.second;
        int c = v.first - minX;
        answer[r][c] = '*';
    }
    return answer;
}
```

- 기존 int타입으로 했을때 틀려서 long long으로 변경해서 해결
- string 변수명(길이, 패턴) 으로 문자열을 만들 수 있다는것을 처음 알게되었다.