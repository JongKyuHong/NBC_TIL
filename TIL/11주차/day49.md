![](https://velog.velcdn.com/images/kyu_/post/4a4347d1-cfc1-4cc9-a73a-6843948abd41/image.png)

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

# 코드카타

## 광물캐기

```c++
#include <string>
#include <vector>
#include <unordered_map>
#include <iostream>

using namespace std;

int answer;

void dfs(vector<int>& picks, int index, int remain, int current, vector<string>& minerals, vector<unordered_map<string, int>>& fatigue, int total, int total_picks){
    if (index >= minerals.size() || (total_picks == 0 && remain == 0)){
        answer = min(answer, total);
        return;
    }
    if (remain > 0){
        // 현재 곡괭이로 광물을 캤을때의 피로도
        dfs(picks, index+1, remain-1, current, minerals, fatigue, total + fatigue[current][minerals[index]], total_picks);
    } else {
        // 곡괭이를 다썼다면 다른거로 바꾸는 시간
        for (int i = 0; i < 3; i++){
            if (picks[i] > 0){
                picks[i]--;
                dfs(picks, index, 5, i, minerals, fatigue, total,total_picks - 1);
                picks[i]++;
            }
        }
    }
}

int solution(vector<int> picks, vector<string> minerals) {
    answer = 10000;
    vector<unordered_map<string, int>> fatigue(3);

    fatigue[0]["diamond"] = 1;
    fatigue[0]["iron"] = 1;
    fatigue[0]["stone"] = 1;
    fatigue[1]["diamond"] = 5;
    fatigue[1]["iron"] = 1;
    fatigue[1]["stone"] = 1;
    fatigue[2]["diamond"] = 25;
    fatigue[2]["iron"] = 5;
    fatigue[2]["stone"] = 1;

    int total_picks = 0;
    for (int i = 0; i < 3; i++){
        total_picks += picks[i];
    }

    for (int i = 0; i < 3; i++){
        if (picks[i] > 0){
            picks[i]--;
            dfs(picks, 0, 5, i, minerals, fatigue, 0, total_picks - 1);
            picks[i]++;
        }
    }

    return answer;
}
```

- [문제](https://school.programmers.co.kr/learn/courses/30/lessons/172927)
- 그냥 완전탐색으로 풀었다.
- 광물을 5개세트로 보고서 묶었으면 더 효율적일거같다.
- 그리디로 풀 수도 있다고 한다.
