# 프로젝트

## 앉으면서 걷기

- 튜터님들에게 질문해서 해결하였다.
- 우선 애니메이션에서 기즈모같이 생긴 기준점이 따라가지않으면 잘못된 애니메이션이다.
  - 원점이 캐릭터를 따라가야 사용할 수 있다.
- Main State -> Control Rig -> Output Pose로 연결되어있는데 Control Rig를 제거하니까 정상적으로 애니메이션이 작동했다.
- 블렌드가 잘못된듯?? 우선은 빼고진행
- 애니메이션창에서 잘나오면 캐릭터에도 잘 적용되어야 한다. -> 다른 곳을 의심해보자

### Control Rig

- IK 즉, 발의 위치를 보정해줌
- 원본 애니메이션이 발을 들어올릴때도 동작을해서 땅에 발을 박아버리는 현상이 발생했던것
- 우선은 Control Rig를 제거하고 사용하기로 하였다.

<iframe width="560" height="315"
src="https://www.youtube.com/embed/eC23EVHUKfs"
title="YouTube video player" frameborder="0"
allow="accelerometer; autoplay; clipboard-write; encrypted-media; gyroscope; picture-in-picture"
allowfullscreen></iframe>

## 앉으면서 다른 방향으로 걷기

![](https://velog.velcdn.com/images/kyu_/post/986886cc-35f4-4f85-984d-42f2cffca5d9/image.png)

- Horizontal은 Direction으로 -180 ~ 180
- Vertical은 GroundSpeed로 0 ~ 300
- 0, 0 (Idle)
- 0, 300(Fwd) : 앞으로 전진
- -90, 300(Left) : 왼쪽이동
- 90, 300 (Right) : 오른쪽 이동
- -180, 300 / 180, 300 (Bwd) : 뒤로 이동

<iframe width="560" height="315"
src="https://www.youtube.com/embed/dPvlrVXcxzo"
title="YouTube video player" frameborder="0"
allow="accelerometer; autoplay; clipboard-write; encrypted-media; gyroscope; picture-in-picture"
allowfullscreen></iframe>

## 엎드리기

![](https://velog.velcdn.com/images/kyu_/post/65d4cf4e-589f-433d-8012-b44e28e8d275/image.png)

- 엎드리기 애니메이션도 리타겟팅을하고
- 블렌드 스페이스를 만들어주었다.
- 근데 동작을 안했다.

### 동작안할때 체크리스트

- GA설정을 했는가?
  - Montage설정을 해주었는가?
  - 필요시 태그를 변경했는가
- GE에서 설정을 했는가?
  - 제대로 Effect설정을 했는가??
- Mapping Context에 제대로 매핑되어있는가?
- ABP설정이 되어있는가?
- 캐릭터에서 Input Action등록 하였는가?
- 캐릭터 Default Abilities에 GA를 등록 하였는가?

- 나같은 경우는 마지막 Default Abilities에 등록을 빼먹어서 안되었다.

### Default Abilities

```c++
UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "GAS|Abilities")
TArray<TSubclassOf<UGameplayAbility>> DefaultAbilities;
```

- ASC를 달아준 액터가 기본적으로 달고있는 어빌리티 목록정의
- GAS는 어빌리티 부여과정이 필요하다 (내코드에서 GiveDefaultAbilities함수)
- 리스트에 있는 어빌리티를 하나씩 꺼내서 ASC->GiveAbility로 등록

<iframe width="560" height="315"
src="https://www.youtube.com/embed/jTNezxWaPwE"
title="YouTube video player" frameborder="0"
allow="accelerometer; autoplay; clipboard-write; encrypted-media; gyroscope; picture-in-picture"
allowfullscreen></iframe>

## 구르기

- 구르기 애니메이션은 조금 달랐다.
- 믹사모에서 In Place를 체크해서 제자리로 만들고 가져왔는데 토글 형태가 아니기 때문에 GA설정이 조금씩 달랐다.
- 또, 구를때 회피를 해야하기때문에 AnimMontage가 필수인데 Default Slot을 쓰면 Blend Space를 AnimMontage가 잡아먹어서 캐릭터가 제대로 움직이지 않았다.
  - 앉거나 엎드렸을때 Idle형태로만 움직여짐
- 그래서 AnimMontage를 우선 포기하고 GA에서 GameplayEffect를 직접 불러서 사용해보기로 하였다.

<iframe width="560" height="315"
src="https://www.youtube.com/embed/jTNezxWaPwE"
title="YouTube video player" frameborder="0"
allow="accelerometer; autoplay; clipboard-write; encrypted-media; gyroscope; picture-in-picture"
allowfullscreen></iframe>

- Main State -> Default Slot -> Output Pose 형태로 연결이 되었는데 움직임이 제대로 동작하였다.
- 하지만 GE가 동작을 안했다. 이동속도가 느려지지 않음
- 사실 기존에도 CharacterMovement의 Crouch를 써서 이동속도가 줄어들었던거지 이전에도 GE는 동작을 제대로 안하고 있었다.
- 그 이유를 찾아야됨

### 느려지지 않는 이유를 찾아보자

- 우선 힌트가 여러개 있었다.

![](https://velog.velcdn.com/images/kyu_/post/8235891c-baf7-43ed-9c5e-ad13cdc3b325/image.png)

- AttributeSet은 제대로 출력됨

![](https://velog.velcdn.com/images/kyu_/post/b8736ead-5f8e-46bf-9a0b-380deb1871ee/image.png)

- 속도가 0임 (느려지지는 않음)

- 원인은 PostGameplayEffectExecute에서 이동속도를 변경해서 그랬다.

- PostGameplayEffectExecute는 GE가 적용된 직후에 발생
- 보통 Instant에 사용한다.

- PostAttributeChange는 수치가 바뀌고 직후, Infinite에도 사용가능
- 이것으로 적용하니 정상적으로 변했다.

- GE에서 Multiply도 Compound로 변경해서 속도가 제대로 적용되었다.

<iframe width="560" height="315"
src="https://www.youtube.com/embed/b4DeiWN3QrA"
title="YouTube video player" frameborder="0"
allow="accelerometer; autoplay; clipboard-write; encrypted-media; gyroscope; picture-in-picture"
allowfullscreen></iframe>

## 다음에 할것

- 앉은 상태에서 엎드리기 키를 누르면 엎드리기가 '예약'되어있다.
- 그 상태에서 앉기를 다시 누르면 일어나야하는데 엎드린다
- 앉은 태그에서 엎드리기키가 들어오면 엎드리기로 전환하고 그 반대도 가능하게 변경해보자
- 구르기를 하면 앉기, 엎드리기 태그도 다 사라지는건 어떨까??
- 구를때 회피도 가능하게 끔 (무적으로 만들면 될듯??)
- 다 했다면 남은 다른 동작들을 추가해보자
- 그리고 가능하다면 동작간 전환 애니메이션도 가지고있는데 그것도 적용해보자
