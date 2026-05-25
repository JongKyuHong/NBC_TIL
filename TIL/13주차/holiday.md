#

## 꾹눌렀을때 공격 가능하게

```c++
void APlayerCharacter::OnAttackStarted(const FInputActionValue& Value)
{
    FGameplayTagContainer AbilityTags;
    AbilityTags.AddTag(FGameplayTag::RequestGameplayTag(FName("Ability.Player.Attack")));
    GetAbilitySystemComponent()->TryActivateAbilitiesByTag(AbilityTags);
}
```

- FGameplayTagContainer라는 태그를 담을 수 있는 컨테이너를 만든다.
  - 컨테이너는 여러개의 태그를 담을 수 있다.
  - FGameplayTag는 하나의 태그만 가능

- RequestGameplayTag를 통해 엔진에 등록된 태그 리스트에서 Ability.Player.Attack이라는 이름을 가진 태그를 찾아온다.
- AddTag로 컨테이너에 등록
- TryActivateAbilitiesByTag : 약간 웹에서 ByName 머그런것들 처럼 태그명으로 어빌리티를 찾는다.
  - 해당 태그를 가지고 있는 어빌리티를 찾아서 실행한다.

### 이렇게 하는 이유??

- C++클래스를 직접 호출하면 로직이 바뀔때마다 C++ 코드를 수정해야 한다.
- 하지만 태그방식을 사용하면 나중에 기획자나 비 개발자가 블루프린트로 새로운 어빌리티를 만들고 태그만 붙여주면 C++코드를 한줄도 안바꾸고 어빌리티를 교체할 수 있다.

### d
