![](https://velog.velcdn.com/images/kyu_/post/6e749833-c813-4493-aba1-4e5d72bbd4e6/image.jpg)

# GAS Ability

- 목표 : Gameplay Tags로 Ability를 제어하고 Effect를 적용할 수 있다.

## Gameplay Ability

- 캐릭터가 수행할 수 있는 행동(Action)을 캡슐화한 시스템
- 실제 게임에서는 '공격'이라는 개념이 아래 로직을 포함한다.
  - Animation (애니메이션 재생, Montage재생)
  - Gameplay Effect(데미지 적용)
  - Gameplay Tags (조건 체크, 상태 관리)
  - Cost(스태미너 소모)
  - Cooldown (쿨다운 대기)

- EnhancedInput과 연결해서 사용가능

```c++
FGameplayTagContainer AbilityTags;
AbilityTags.AddTag(FGameplayTag::RequestGameplayTag(FName("Ability.Attack.Melee")));

AbilitySystemComponent->TryActivateAbilitiesByTag(AbilityTags);
```

- 실행할 태그를 정의하고,
- AbilitySystemComponent->TryActivateAbilitiesByTag(AbilityTags); 정의한 태그(Ability.Attack.Melee)를 가진 Ability중 실행 가능한 걸 찾아서 실행
- 만약에 캐릭터에서 실행되는 GA라면 캐릭터 ASC에 등록을 해주어야 한다.

## Gameplay Ability의 생명주기

- Inactive 상태
- TryActivateAbility() 호출
- CanActivateAbility() 체크
  - Tags 조건 확인
  - Cost 충분한지 확인
  - Cooldown 완료됐는지 확인
- ActivateAbility() 실행
  - Montage 재생
  - Effect 적용
  - Tags 추가
- Active 상태
  - Montage 완료 또는 취소
- EndAbility() 호출
  - Tags제거
  - 정리 작업

### 주요 함수

- CanActivateAbility()
  - Ability 활성화 가능한지 체크
  - Tags, Cost, Cooldown 확인
  - 자동 호출(오버라이드 가능)

- ActivateAbility()
  - Ability시작
  - 여기에 실제 로직 (Montage 재생, Effect 적용 등)
  - 반드시 오버라이드 해서 사용

- EndAbility()
  - Ability종료
  - Tags 제거, 정리 작업
  - 반드시 호출(Montage 완료 시)

## Gameplay Tags와 Ability

- Activation
- Required Tags : 이 Tag가 있어야 Ability 발동가능, State.Grounded -> 땅에 있어야 점프 가능
- Blocked Tags : 이 Tag가 있으면 Ability 발동 불가, State.Attacking -> 공격중이면 공격 불가
- Owned Tags : Ability 활성화 시 자동으로 추가되는 Tag, State.Attacking -> 공격 중임을 표시
- Cancel Abilities with Tag : 이 Tag를 가진 다른 Ability 취소, Action.Move -> 이동 Ability 취소
- Block Abilities with Tag : 이 Tag를 가진 다른 Ability 발동 방지, Action.Attack -> 다른 공격 Ability 블록

## Gameplay Event vs AnimNotify

- AnimNotify
  - Chracter Blueprint에서 처리
  - 로직이 분산됨

- Gameplay Event
  - Ability C++에서 처리
  - 로직이 통합됨
  - 디커플링

### Gameplay Event를 사용하는 이유

- AnimNotify를 쓰면 캐릭터의 블루프린트나 코드가 비대해짐
- Gameplay Event를 쓰면 애니메이션은 그냥 신호만 쏘고, 진짜 로직은 Ability쪽에서 처리해서 구조가 가벼워 진다.

- AnimNotify는 애니메이션 타임라인 위에서 특정 시점에서 바로 함수를 실행시키는 기능
  -> 공격 모션 중간에 AN_AttackHitCheck -> 캐릭터 C++/BP에서 직접 히트 판정 함수 호출
  -> 발소리 노티파이 -> 캐릭터에서 발소리 재생
  -> 이펙트 노티파이 -> 캐릭터에서 파티클 스폰

이런식인데

- Gameplay Event를 사용하면
  애니메이션에서 어떤 태그를 가진 이벤트를 쏨 -> Ability쪽에서 그 태그를 기다렸다가 처리하는 구조
  애니메이션에 커스텀 노티파이를 사용하고 -> 이 노티파이가 GameplayEvent를 GameplayAbility에다가 쏜다 -> Ability에서 실행

## Ability Instancing Policy

```c++
UGA_MeleeAttack::UGA_MeleeAttack()
{
    // Ability Instancing Policy
    InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;

    // Ability Tags
    AbilityTags.AddTag(FGameplayTag::RequestGameplayTag(FName("Ability.Attack.Melee")));

    // 공격 중이면 발동 불가
    ActivationBlockedTags.AddTag(FGameplayTag::RequestGameplayTag(FName("State.Attacking")));

    // 발동 시 State.Attacking 추가
    ActivationOwnedTags.AddTag(FGameplayTag::RequestGameplayTag(FName("State.Attacking")));
}
```

- Ability Instancing Policy는 이 GameplayAbility가 언제, 몇개의 UObject 인스턴스로 만들어져 실행되는가를 정하는 옵션, 능력이 발동될 때 메모리에 이 능력의 복사본을 어떻게 생성하고 관리할 것인가
- NonInstanced(비인스턴스)
  - 능력 인스턴스를 만들지 않고 클래스의 CDO(기본객체, 설계도를 그냥 씀)로 실행, 별도의 복사본 안만듬
  - 메모리 소모 거의 없다, 성능이 빠르다
  - 모든 캐릭터가 공유하는 설계도와 같아서 변수를 저장할 수 없다. (설계도를 건드리는 꼴)
  - 실행 즉시 끝나는 단순한 능력일때 (즉시 체력 회복)에 적합하다.

- Instanced Per Actor (액터당 하나씩 생성)
  - 능력을 가진 캐릭터마다 해당 능력의 복사본을 딱 하나만 만듬
  - 캐릭터별로 고유한 변수 상태를 저장 가능
  - 캐릭터가 많아지면 메모리 사용량이 약간 늘어난다.
  - 대부분의 일반적인 상황에서 사용

- Instanced Per Execution(실행할 때마다 생성)
  - Activate할때마다 새 UObject인스턴스 만들어서 생성
  - 간섭이 없다.
  - 메모리 오버헤드가 가장 크다. 매번 생성-파괴 해야하기 때문
  - 아주 특수한경우에만 사용

## Ability Task

- Ability 내에서 시간이 걸리는 작업을 처리하는 특수 객체
- 비동기 : 백그라운드에서 실행되며 완료 시 콜백
- Ability와 생명주기 공유 : Ability종료 시 Task도 자동 정리
- 여러 개 동시 실행 가능 : Montage 재생 + Event 대기 동시에

```c++
void UGA_MeleeAttack::ActivateAbility(...)
{
    // 1. Task 생성
    UAbilityTask_PlayMontageAndWait* MontageTask =
        UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(...);

    // 2. 콜백 바인딩 (완료/취소/중단 시 호출될 함수)
    MontageTask->OnCompleted.AddDynamic(this, &ThisClass::OnMontageCompleted);
    MontageTask->OnCancelled.AddDynamic(this, &ThisClass::OnMontageCancelled);

    // 3. Task 활성화 (이 시점부터 실행 시작)
    MontageTask->ReadyForActivation();

    // 4. 여러 Task 동시 실행 가능
    UAbilityTask_WaitGameplayEvent* EventTask =
        UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(...);
    EventTask->EventReceived.AddDynamic(this, &ThisClass::OnDamageEvent);
    EventTask->ReadyForActivation();

    // 함수는 여기서 끝나지만, Task들은 계속 실행됨!
}

void UGA_MeleeAttack::OnMontageCompleted()
{
    // Montage 완료 시 호출됨 (2초 후)
    // Ability 종료 → 모든 Task 자동 정리
    	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}
```

- Task는 그냥 생성하고 활성화 하면 된다.
- 직접삭제하면 안됨, 알아서 EndAbility()에서 정리됨


## 결론

Gameplay Ability, Gameplay Event를 쓰면 기존처럼 BP_Character에다가 AnimMontage를 직접 연결해줄 필요가 없다.
ASC에 Ability를 등록해주자.
Ability에서 어떤 태그가 디폴트인지, 뭐 Require, Blocked같은거 설정해줄 수 있음

커스텀 Nofity

- 애니메이션 타임라인에 커스텀 Notify달아줌
- BP에서 설정해줌
![](https://velog.velcdn.com/images/kyu_/post/0afd1afa-da58-4031-9404-6a2c40f6e5f3/image.png)

- Received Notify (mesh Comp로 get Owner) -> 캐릭터 연결 -> Send Gameplay Event to Actor (Event Tag연결) -> Return Node
- 흐름 : 애니메이션 재생 중 특정 타임라인에서 커스텀 Notify발생 -> Notify가 캐릭터에게 신호를 보냄(나 발생했음!) -> 캐릭터가 이 신호를 받아서 Gamepla Event를 발생시킴 -> GAS Ability가 이 이벤트를 기다리고 있음 -> 자동 실행