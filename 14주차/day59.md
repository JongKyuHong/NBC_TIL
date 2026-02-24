# Gameplay Cue(GC)

- 시각적, 청각적 연출(VFX, SFX, 카메라 쉐이크, 포스트 프로세스)을 전담하는 시스템
- 게임플레이 로직은 서버에서 처리하되 그에 맞는 리액션(화면 흔들리기 피튀기기 같은 연출)은 클라이언트에서 GameplayCue.Hit태그를 줄테니까 알아서 처리 ㄱㄱ
- 이런식으로 구조를 짜면 네트워크 트래픽을 아낄수 있다.
- **시각적 청각적 효과만 담당하므로 시스템 로직에 영향이 없다**
- 하나의 게임플레이 태그가 하나의 게임플레이 큐에 대응하는 구조
- 독립된 GameplayCue 관리자가 태그를 기반으로 FX효과를 재생한다.

## 종류

- Static : 일순간의 단발성 FX
- Actor : 살아있는동안 지속, 반복되는 FX

### GameplayEffect와의 연계

- Instant 타입 이펙트 -> GameplayCue Static이 적합
- Duration -> GameplayCue Actor가 적합

- Event Functions
- GameplayCue Static : 발동 시 OnExecute 함수 실행
- GameplayCue Actor : 반복되는 주기(Period)마다 Executed 이벤트를 발생시킴

## 어떤식으로 호출??

- 게임플레이큐를 실행하려면 ASC를 통해 GameplayCueManager에게 GameplayTag와 `GameplayCue`라는 필수 부모이름, 이벤트 유형 (실행, 추가, 제거)을 전송해야 한다.
- 게임 플레이 큐는 GameplayCue로 시작하는 게임플레이 태그를 붙여야 한다.

## 예시

- 만약 총알에 맞았다고 가정해보자
- 총알은 직접 화면을 흔들라는 명령을 하지 않음
- 총알은 실제로 데미지만 입히고 빠지며 데미지 입은 쪽이 데미지입었네?하고 GC를 실행하는 구조
- 데미지 입는 GE가 실행될때 GE에서 자동으로 호출하는 방법이 있고,
- AttriebuteSet에서 수동으로 호출도 가능
  - 데미지에 따라서 카메라 흔들림 강도를 정하려면 이쪽이 더 적합해 보인다.
  - ExecuteGameplayCue를 호출

# 실제 구현해보자

## 주제

- 캐릭터가 데미지를 입으면 화면이 흔들리면서 붉은 색으로 피격효과가 나타나는 것을 만들고자 하였다.

## AttributeSet 수정

```c++
// 캐릭터에게 들어온 데미지를 저장
UPROPERTY(BlueprintReadOnly, Category = "Character|Combat")
FGameplayAttributeData Damage;
ATTRIBUTE_ACCESSORS(UCharacterAttributeSet, Damage);
```

- AttributeSet에 캐릭터에게 들어온 데미지를 저장하는 Attribute를 추가

```c++
// UCharacterAttributeSet::PostGameplayEffectExecute

else if (Data.EvaluatedData.Attribute == GetDamageAttribute())
    {
        // Health에서 바로 깎지 않는 이유는 도트데미지일수도 있어서

        // 들어온 데미지 저장
        const float DamageValue = GetDamage();
        SetDamage(0.f);

        // 데미지가 0이하면 무시
        if (DamageValue <= 0.f) return;

        // 피 보정
        const float NewHealth = FMath::Clamp(GetHealth() - DamageValue, 0.0f, GetMaxHealth());
        SetHealth(NewHealth);

        // GameplayCue에 넘길 파라미터
        FGameplayCueParameters Parameters;
        Parameters.RawMagnitude = DamageValue;

        // 태그를 가진 Cue를 실행
        const FGameplayTag DamageTag = FGameplayTag::RequestGameplayTag(FName("GameplayCue.Character.Damaged"));
        GetOwningAbilitySystemComponent()->ExecuteGameplayCue(DamageTag, Parameters);
    }

```

- 데미지가 들어온 후 그에맞는 로직 수행
- RawMagnitude는 GC에서 받아보는 수치
- GameplayCue.Character.Damaged태그를 가진 GC를 실행한다.
- ExecuteGameplayCue는 찾아낸 Cue의 OnExecute이벤트를 실행시킨다.

## BP클래스 만들기

![](https://velog.velcdn.com/images/kyu_/post/433e73f3-93ae-4289-8e17-fd9d88338516/image.png)

![](https://velog.velcdn.com/images/kyu_/post/60f5d204-afe4-4ca4-a722-198157e9d607/image.png)

- GameplayCueNotify_Actor를 상속받는 BP_GCN_HitReaction라는 블루프린트 클래스를 만듬

![](https://velog.velcdn.com/images/kyu_/post/6e2f3498-8258-4ce9-a423-19463d97f998/image.png)

- GameplayCueTag를 우리가 만든 태그로 지정

![](https://velog.velcdn.com/images/kyu_/post/639bd14a-a320-4229-85bc-c6d8dfdd1ae8/image.png)

![](https://velog.velcdn.com/images/kyu_/post/75895052-ddb3-45c1-bfe5-820206c9f254/image.png)

- CameraShakeBase를 선택해서 BP_HitReaction_CameraShake라는 블루프린트 클래스를 만들었다.

![](https://velog.velcdn.com/images/kyu_/post/5fa19ff5-1082-4455-b4a8-84e3da6345e4/image.png)

- Root Shake Pattern에서 Wave Oscillator Camera Shake Pattern을 선택
  - 그냥 진동의 한 종류
- 아래 Location, Rotation, FOV, timing을 설정해주었다.

## 피격 머티리얼 만들기

![](https://velog.velcdn.com/images/kyu_/post/c8622bf0-0df4-40dc-83fa-ed841a70f3d7/image.png)

## GameplayCueNotify만들기

- BP_GCN_HitReaction에서

![](https://velog.velcdn.com/images/kyu_/post/a7421f3f-0189-4382-b6ea-2b9ae4ed801f/image.png)

- Function에서 On Execute를 선택
  - 부모클래스인 GameplayCueNotify_Actor에 있으므로 Function의 override에서 찾을 수 있었다.
- OnExecute 함수는 무엇인가?
  - 서버가 클라이언트에게 보내는 호출장같은 느낌 (지금 당장 연출해라)

### OnExecute 블루프린트

- OnExecute함수로부터 캐릭터를 캐스팅
- 캐스팅된 캐릭터로부터 Controller와 Post Process 컴포넌트(보정 필터 역할)를 꺼낸다.
  - BP_Character로 캡슐컴포넌트 아래에 Post Process를 달아주었음
  - Controller의 경우 변수로 승격 (나중에 사용)
- 캐스팅된 캐릭터의 실행핀을 Create Dynamic Material Instance에 연결
  - Parent로 아까 생성한 피격 머티리얼을 등록
  - 변수로 승격해주었다. 변수명 Vignett Material
    - 비네트효과의 그 비네트를 이름으로 달아주었다.
- 캐릭터에서 나온 Post Process와 비네트 머티리얼을 Add or Update Blendable에 연결
  - Post Process는 Target에, 비네트는 In Blendable Object에

#### Add or Update Blendable 노드

- 포스트 프로세스 효과를 실시간으로 끄고 키거나 강도 조절을 하는 노드
- Target : 이 필터를 어디에 적용할지 (우리의 경우 Post Process에)
- In Blendable Object : 어떤 필터를 적용할래? (우리의 경우 비네트 DMI를 연결)

- Add or Update Blendable노드는 플레이어 컨트롤러 노드에 실행핀이 연결되고 끝에 Hit Effect에 연결(커스텀 이벤트 노드)

![](https://velog.velcdn.com/images/kyu_/post/b4b254aa-fbcf-4960-bb3e-67e9066d29ac/image.png)

### Hit Effect 커스텀 이벤트 만들기

- HitEffect의 실행핀을 Client Start Camera Shake에 연결하고 우리가 만든 Shake를 Shake 핀에 달아준다.
- HitEffect의 Inputs으로 Float를 하나 만들어준다.

- Target은 아까 변수로 승격한 Controller
- Timline을 만드는데

![](https://velog.velcdn.com/images/kyu_/post/0c619828-e830-4d43-97ac-72bb5f5785d4/image.png)

- 그냥 단순하게 만들었다. Linear말고 Auto로 지정해서 부드러운 곡선처럼
- Timeline의 Update를 Set scalar Parameter Value 노드에 연결
  - Target은 Vignett Material, value는 Intensity를 연결해주었다.
  - Parameter Name은 Intensity로

#### Set scalar Parameter Value 노드

- 블루프린트에서 머티리얼안에 있는 특정 숫자변수값을 변환시키고 싶을때 사용한다.
- Target : 대상
- Parameter Name : 파라미터 이름 (머티리얼 안에서 내가 만든 Scalar Parameter 노드이름)
- Value : 실제로 변경할 값

![](https://velog.velcdn.com/images/kyu_/post/6b73219b-ab83-40e6-8644-6d1aec6121b9/image.png)

### 데미지에 따라 강조하기??

- Map Range Clamped 노드를 만들고 HitEffect의 Damage를 Value에 연결
- Timeline의 Intensity값을 Map Range Clamped의 Return값과 곱해서 스칼라 파라미터 밸류에 넣어주었다.
- 또, Map Range Clamped의 Return값을 Camera Shake의 Scale값으로도 넣어주었다. (데미지에 따라 강한 흔들림)

#### Map Range Clamped 노드

- 비례식을 쉽게 사용할 수 있는 노드
- A라는 범위의 숫자를 B라는 숫자의 변환하되 범위를 벗어나지 않게 Clamped, 보정해줌
- Value : 변환하고 싶은 값
- In Range A / B : 원본 범위의 최소/최대 (우리는 0, 100)
- Out Range A / B : 변환하고 싶은 범위의 최소/최대 (1, 3으로 지정하면 0일때 1, 100일때 3)

![](https://velog.velcdn.com/images/kyu_/post/228bbd38-b629-4863-a164-8b69f5533674/image.png)

# 테스트

- 테스트용 GE만들기

![](https://velog.velcdn.com/images/kyu_/post/ca005c81-be10-4b31-ad26-a4bbf0b5ebe9/image.png)

- 이름은 BP_GE_DamageTest
- 타겟 Attribute는 Damage로
- 데미지는 20 입게끔

![](https://velog.velcdn.com/images/kyu_/post/d4119629-2675-41a1-a9b7-c9fef5076923/image.png)

- BP_PlayerCharacter에서 G키를 누르면 ApplyGameplayEffectToSelf를 통해 GE가 호출되도록 하였다.
- GE가 호출되면 AttributeSet의 PostGameplayEffectExecute에서 구현해둔 데미지 로직에 의해 GCN이 호출될것이다.

## 흐름

- G키누름 ApplyGameplayEffectToSelf로 BP_GE_DamageTest호출
- BP_GE_DamageTest로 인해 데미지에 변화
- PostGameplayEffectExecute의 로직 수행

```c++
const FGameplayTag DamageTag = FGameplayTag::RequestGameplayTag(FName("GameplayCue.Character.Damaged"));
        GetOwningAbilitySystemComponent()->ExecuteGameplayCue(DamageTag, Parameters);
```

- 위 코드로 인해 GameplayCue.Character.Damaged태그를 가진 GameplayCue가 실행
- BP_GCN_HitReaction의 GameplayCueTag에 GameplayCue.Character.Damaged 이 태그를 지정해두었으므로 BP_GCN_HitReaction의 OnExecute가 실행
- OnExecute가 실행되며 HitEffect 커스텀 이벤트 실행
- 우리가 눈으로 확인가능

## 결과

<iframe width="560" height="315"
src="https://www.youtube.com/embed/t4IKldjv4aY"
title="YouTube video player" frameborder="0"
allow="accelerometer; autoplay; clipboard-write; encrypted-media; gyroscope; picture-in-picture"
allowfullscreen></iframe>
