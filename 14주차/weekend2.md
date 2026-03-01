
## 발소리만들기

- https://velog.io/@kyu_/%EB%B0%9C%EC%86%8C%EB%A6%AC-%EB%A7%8C%EB%93%A4%EA%B8%B0

## GA_Jump 만들기

- https://velog.io/@kyu_/GAJump-%EB%A7%8C%EB%93%A4%EA%B8%B0

# 태그 관련 대폭 수정

## Chracter수정

- PlayerChracter에서 기존에는

```c++
FGameplayTag ReloadTag;
FGameplayTag AttackingTag;
FGameplayTag ProneTag;
FGameplayTag DeathTag;
FGameplayTag CrouchTag;
FGameplayTag RollTag;
FGameplayTag JumpAbilityTag;
```

- 헤더에서 이런식으로 태그를 선언하고

```c++
void APlayerCharacter::BeginPlay()
{
    ReloadTag = FGameplayTag::RequestGameplayTag(FName("State.Player.IsReloading"));
    // 기존에 AttackTag 있어서 다른 이름으로 사용
    AttackingTag = FGameplayTag::RequestGameplayTag(FName("State.Player.IsAttacking"));
    ProneTag = FGameplayTag::RequestGameplayTag(FName("State.Player.IsProning"));
    DeathTag = FGameplayTag::RequestGameplayTag(FName("State.Player.Death"));
    CrouchTag = FGameplayTag::RequestGameplayTag(FName("State.Player.IsCrouching"));
    RollTag  = FGameplayTag::RequestGameplayTag(FName("State.Player.IsRolling"));
    AirborneTag = FGameplayTag::RequestGameplayTag(FName("State.Player.IsInAir"));
    JumpAbilityTag = FGameplayTag::RequestGameplayTag(FName("Ability.Player.Jump"));
}
```

- 비긴플레이에서 이런식으로 초기화 해주었다. 
- 이 방식에는 많은 문제가 있는데 우선 FName안에 TEXT를 써서 형식에 맞게 자동으로 문자열이 변환되게 해야하고, 오타가 나면 제대로 작동하지 않을 가능성이 높다.
- 또, Ability 태그의 경우에는 AbilityTags.AddTag(FGameplayTag::RequestGameplayTag(FName("Ability.Player.Crouch"))); 그냥 이런식으로 직접 호출하는등 문제가 많았다.
- 그래서 이 태그들을 모두 에디터에서 지정해주는 방식으로 변경하고자 하였다.

## 기존 방식과의 차이

- 기존 방식의 문제점
1. 플레이어에서 행동할때마다 RequestGameplayTag를 호출해서 자원소모
2. State태그들은 beginplay에서 초기화했으나 Ability 태그들은 초기화가 없었음
3. 문자열로 직접 수정하니 오타가 있을수도 있고, 캐싱이 제대로 안됨

- 에디터에서 지정하는 방식의 장점
1. UPROPERTY에 등록하면 에셋 데이터에 직렬화되어서 저장, 게임이 켜지고 캐릭터가 로드될때 이 정보는 변수에 박힌채로 들어옴, beginplay가 실행되는 시점에서 추가연산이 없음
2. 오타 방지, 태그매니저에서 직접 지정가능하여 오타가능성이 낮음
3. RequestGameplayTag는 행동마다 붙어있어서 매번 태그매니저 뒤져서 가져왔지만 지금은 그런 추가연산이 없다.

## 예시

### GA_Crouch

- 기존

```c++
UGA_Crouch::UGA_Crouch()
{
	// 어빌리티가 실행될때 액터당 하나의 인스턴스(객체)만 생성해서 재사용하겠다.
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
	// Ability Tags, 어빌리티 자체에 붙여주는 태그
	AbilityTags.AddTag(FGameplayTag::RequestGameplayTag(FName("Ability.Player.Crouch")));
	
	// 앉기 어빌리티가 켜질때 엎드리기상태였다면 엎드리기 어빌리티가 실행중이라면 취소
	CancelAbilitiesWithTag.AddTag(FGameplayTag::RequestGameplayTag(FName("Ability.Player.Prone")));
	
	// 이태그가 있으면 실행안함 (앉아있을때 또 앉을 수 없음)
	ActivationBlockedTags.AddTag(FGameplayTag::RequestGameplayTag(FName("State.Player.IsCrouching")));
	ActivationBlockedTags.AddTag(FGameplayTag::RequestGameplayTag(FName("State.Player.Death")));

	// 소유태그 / 실행중 어떤 태그를 가질지
	ActivationOwnedTags.AddTag(FGameplayTag::RequestGameplayTag(FName("State.Player.IsCrouching")));
}

```

- 생성자에서 직접 태그를 달아주는 방식이였다. 이를 에디터에서 달아주는 방식으로 변경했다.

![](https://velog.velcdn.com/images/kyu_/post/0c303662-a972-4f15-a2f3-4e389e26b06b/image.png)



### CanJump

- CanJump 기존 코드이다.

```c++
bool APlayerCharacter::CanJump() const
{
  if (!AbilitySystemComponent) return false;

  static FGameplayTagContainer JumpBlockTags;

  if (JumpBlockTags.IsEmpty())
  {
    JumpBlockTags.AddTag(ReloadTag);
    JumpBlockTags.AddTag(AttackTag);
    JumpBlockTags.AddTag(ProneTag);
    JumpBlockTags.AddTag(CrouchTag);
    JumpBlockTags.AddTag(RollTag);
  }
return !AbilitySystemComponent->HasAnyMatchingGameplayTags(JumpBlockTags);
}

```

- 기존에는 BeginPlay에서 정의했던 태그들을 컨테이너에 담아서 하나라도 가지고있으면 점프를 못하게 하는 방식을 사용했다.
- static을 사용하는것이 anti-pattern이여서 고민이 많이 되었다.

#### 개선 후

```c++
// 헤더
UPROPERTY(EditAnywhere, Category = "GAS | Block Tags")
FGameplayTagContainer JumpBlockTags;

// 구현
bool APlayerCharacter::CanJump() const
{
    if (!AbilitySystemComponent) return false;
    return !AbilitySystemComponent->HasAnyMatchingGameplayTags(JumpBlockTags);
}
```

- 코드가 간결해졌다.
- 헤더에서 UPROPERTY를 달아주고

![](https://velog.velcdn.com/images/kyu_/post/de037552-beef-41be-bb0f-da46ca762862/image.png)

- 에디터에서 직접 지정해주는것이 오타도 방지되고 훨씬 편하다고 느꼈다.

![](https://velog.velcdn.com/images/kyu_/post/886c5fbb-925e-4b55-9828-23ac62db9b1a/image.png)

- BP_PlayerCharacter에서 태그들을 직접 에디터에서 달아주었다.

#### 2차 개선

- 생각해보니 Jump의 경우는 GA와 GE가 생겨서 이제굳이 코드에서 CanJump가 필요가없다. JumpBlockTags도 지워주고 GA에서 BlockedTag에다가 등록을 하자

![](https://velog.velcdn.com/images/kyu_/post/b435de86-b896-4186-b43b-feb63f502441/image.png)
