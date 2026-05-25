# 팀프로젝트

## 동작 간 자연스러운 전환

```c++
UGA_Roll::UGA_Roll()
{
CancelAbilitiesWithTag.AddTag(FGameplayTag::RequestGameplayTag(FName("Ability.Player.Prone")));
CancelAbilitiesWithTag.AddTag(FGameplayTag::RequestGameplayTag(FName("Ability.Player.Crouch")));
}
```

- CancelAbilitiesWithTag : 이 태그를 가진 어빌리티는 취소시킨다.
- 구르기에서 앉기, 엎드리기 기본태그를 달아놓아서 구를때는 엎드리기, 앉기가 풀리도록 설정하였다.
- 앉기에서는 엎드리기를 취소, 엎드리기에서는 앉기가 취소되도록 설정

## 구르기 동안 무적설정

- Anim Montage를 사용하면 AN을 통해서 특정시점에 무적을 걸어줄 수 있지만 우선은 Montage를 못쓰므로 GE에서 설정해주었다.
- BP_GE_Roll에서

![](https://velog.velcdn.com/images/kyu_/post/e42e5c15-02a1-4f89-b49b-9a0045eb9b25/image.png)

- Any Tags Match를 설정하고

![](https://velog.velcdn.com/images/kyu_/post/1adaf1c1-94d0-4faf-b87c-f8965a0a454c/image.png)

- Owning Tag Query에서 우선 Effect.Damage라는 태그를 만들어서 넣어주었다.
- 추후에 데미지를 입히고 입는 기능이 생겼을때 수정하면 될 것 같다.
- 또, 구르기 시간에 맞게 Has Duration을 0.5로 설정하였다.

## 동작중 콜리전 변경하기

![](https://velog.velcdn.com/images/kyu_/post/1065fa3c-98eb-4fb5-a9d5-e0db4d17e8b8/image.png)

- 앉은 상태인데 콜리전이 서있는 상태와 비슷하다 이를 해결해보자

```c++
Character->GetCapsuleComponent()->SetCapsuleHalfHeight(60.f);
	Character->GetMesh()->SetRelativeLocation(FVector(0.f, 0.f, -60.f));
	FGameplayEffectContextHandle EffectContext = MyASC->MakeEffectContext();
	EffectContext.AddSourceObject(Character);
```

- GA_Crouch의 ActivateAbility에서 이렇게 콜리전을 변경하는 코드를 추가해 주었다.
- 앉기는 보통 2/3정도로 설정한다고 해서 2/3로 설정해주었다.
- SetRelativeLocation는 캡슐기준으로 메시의 위치를 잡음
  - 높이가 변경되었기 때문에 Z값을 높이와 같이 맞춰줌(기준점이 캐릭터의 중앙에 위치해있기때문에 이것을 설정해주지 않으면 땅에 발이 들어가있게보인다)
- 엎드리기는 높이의 1/3으로 설정해주었다.

<iframe width="560" height="315"
src="https://www.youtube.com/embed/78kaoovSXGw"
title="YouTube video player" frameborder="0"
allow="accelerometer; autoplay; clipboard-write; encrypted-media; gyroscope; picture-in-picture"
allowfullscreen></iframe>

## 장전

- 장전 포즈도 리타겟팅해서 불러왔다.
- 앉아있을때, 엎드릴때, 서있을때 세가지 애니메이션이 있는데
- 서있을때 장전을하면 장전 후에 아주 잠깐 A포즈가 나왔다.

<iframe width="560" height="315"
src="https://www.youtube.com/embed/z3mRQlmvI_Y"
title="YouTube video player" frameborder="0"
allow="accelerometer; autoplay; clipboard-write; encrypted-media; gyroscope; picture-in-picture"
allowfullscreen></iframe>

- OnCompleted를 OnBlendOut으로 변경해서 해결하였다.

```c++
// 바꾸기전
MontageTask->OnCompleted.AddDynamic(this, &UGA_Reload::OnMontageEnded);

// 바꾼 후
MontageTask->OnBlendOut.AddDynamic(this, &UGA_Reload::OnMontageEnded);
```

- OnCompleted : 몽타주가 끝날때까지 기다림
- OnBlendOut : 몽타주가 끝나기 전 (blend time out)에 신호를 주고 몽타주가 서서히 사라지면서 EndAbility가 실행된다고 한다.
