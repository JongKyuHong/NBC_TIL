## 공격만들기

- 리타겟팅은 이제 편하게 가능
- 맨손 공격도 있을 수 있어서 네이밍은 Attack으로 통일했다.
- 무기의 경우 연발/단발이 있는데 모드로 설정가능보다는 무기자체가 연발무기 / 단발무기로 나누는것으로 얘기했던거같아서 그렇게 개발하기로 하였다.

### GA

- 단발 무기인지 연발무기인지 판단을 생성자에서 두었는데 무기가 변경될때 GA가 새로생기는게 아니기때문에 다른 방법을 사용해야했다.

```c++
virtual bool CanActivateAbility(
		const FGameplayAbilitySpecHandle Handle,
		const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayTagContainer* SourceTags = nullptr,
		const FGameplayTagContainer* TargetTags = nullptr,
		FGameplayTagContainer* OptionalRelevantTags = nullptr
		) const override;
```

- CanActivateAbility 는 UGameplayAbility의 기본제공 함수이다.
- 지금 어빌리티를 실행해도 되는지 판단하는 함수
- TryActivateAbility가 실행되면 제일먼저 체크하는 함수라고 한다.
- OptionalRelevantTags : CanActivateAbility가 false를 반환했을때 왜 실행이 안되었는지에 대한 추가정보를 담아서 호출자에게 전달할 때 사용

```c++
bool UGA_Attack::CanActivateAbility(
	const FGameplayAbilitySpecHandle Handle,
		const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayTagContainer* SourceTags,
		const FGameplayTagContainer* TargetTags,
		FGameplayTagContainer* OptionalRelevantTags) const
{
	if (!Super::CanActivateAbility(Handle, ActorInfo, SourceTags, TargetTags, OptionalRelevantTags))
	{
		return false;
	}

	// const 안정성을 위해서 매개변수의 ActorInfo를 직접 쓰는게 좋다고 한다.
	APlayerCharacter* Player = Cast<APlayerCharacter>(ActorInfo->AvatarActor.Get());
	if (!Player) return false;

	// 이 부분은 나중에 무기정보같은곳에서 연사인지 단발인지 판단을 해서 분기하면 될듯
	if (!bIsRepeat)
	{
		FGameplayTag AttackingTag = FGameplayTag::RequestGameplayTag(FName("State.Player.IsAttacking"));
		if (ActorInfo->AbilitySystemComponent->HasMatchingGameplayTag(AttackingTag))
		{
			return false;
		}
	}

	return true;
}
```

- 우선은 테스트용으로 bIsRepeat이라는 멤버변수로 판단하는 식으로 코드를 짰다.
- 추후에 무기 정보에 따라서 bIsRepeat부분을 바꿔주면 될듯

### GA는 어디까지 구현해야 하는가

- 우리는 프로젝타일 방식으로 구현을 한다.
- 프로젝타일은 발사체 생성, 데미지를 입히는 시점이 분리되어있다.
- 이 구조에서 GA는 어디까지 역할을 담당하는지 고민이 되었다.
- 우선은 애니메이션 재생, 코스트 소모, 태그 관리, 컴포넌트에게 발사명령? 내리기 정도라고 판단을 하였다.
