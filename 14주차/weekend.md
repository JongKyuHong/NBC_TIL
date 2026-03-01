# 발소리

- 발소리를 만들어보자
- 대략적으로 저번에썼던 GameplayCue를 사용하면 소리를 재생할 수 있겠다고 생각을 했다.
- 그래서 전체적인 구조를 이렇게 짜보았다.

# 구조

1. AnimNotify를 애니메이션 시퀀스에 달아준다.
2. AnimNotify에서 태그로 GameplayCue에 해당하는 태그를 지정해줌
3. 발이 닿았을때 GameplayCue가 호출되어서 실행

# 구현

## AN_FootStep.h

```c++
class FINALMINUTES_API UAN_FootStep : public UAnimNotify
{
	GENERATED_BODY()
public:
	UAN_FootStep();

	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="GAS")
	FGameplayTag FootStepCueTag;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="GAS")
	float TraceDistance;
};
```

- UAnimNotify를 상속해서 만들었다.
- Notify라는 가상함수를 override
  - Notify함수가 두개가 있는데 AnimNotify.h에 들어가서 보니 인자가 두개인 Notify는 DEPRECATED되어있어서 인자가 세개인 Notify를 채택했다.
- GameplayCue를 호출할 FootStepCueTAg를 리플렉션 시스템에 등록
  - 생성자에서 초기화해주지만 에디터에서도 지정할수 있도록
- TraceDistance는 발아래로 라인트레이스를 쏴서 재질이 무엇인지 파악할것인데 이또한 리플렉션 시스템에 등록했다.

## AN_FootStep.cpp

```c++
UAN_FootStep::UAN_FootStep()
{
    // GameplayCue 태그설정
    FootStepCueTag = FGameplayTag::RequestGameplayTag(FName("GameplayCue.Character.Footstep"));
    // 바닥까지 탐색 거리
    TraceDistance = 100.f;
}

void UAN_FootStep::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
    Super::Notify(MeshComp, Animation, EventReference);

    if (!MeshComp) return;
    AActor* Owner = MeshComp->GetOwner();
    if (!Owner) return;

    FHitResult HitResult;
    // Mesh아래쪽으로 100만큼 라인트레이스 탐색
    FVector Start = MeshComp->GetComponentLocation();
    FVector End = Start - FVector(0.f, 0.f, TraceDistance);

    // 자기 자신은 안맞게 Ignore설정
    FCollisionQueryParams QueryParams;
    QueryParams.AddIgnoredActor(Owner);
    // 맞은 물체의 물리재질정보를 가져오겠다 true
    QueryParams.bReturnPhysicalMaterial = true;
    // 복잡한 콜리전도 체크
    QueryParams.bTraceComplex = true;

    // 라인트레이스
    bool bHit = Owner->GetWorld()->LineTraceSingleByChannel(
        HitResult, Start, End, ECollisionChannel::ECC_Visibility, QueryParams
    );

    if (bHit && HitResult.PhysMaterial.IsValid())
    {
        UAbilitySystemComponent* ASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(Owner);
        if (!ASC) return;

        FGameplayCueParameters Params;
        // 맞은 위치
        Params.Location = HitResult.Location;
        // 파티클 추가할때 사용
        Params.Normal = HitResult.ImpactNormal;
        // 맞은 재질
        Params.PhysicalMaterial = HitResult.PhysMaterial;
        // 소리낸 주체
        Params.EffectCauser = Owner;

        // BP_GCN_FootStep에서 재질에 따라 설정된 소리 틀어줌
        ASC->ExecuteGameplayCue(FootStepCueTag, Params);
    }
}
```

- Notify안에는 우리가 익히봐왔던 라인트레이스 쏘는 로직을 넣어놓았다.
- 라인트레이스 로직은 복사해서 사용을하였고, 옵션들만 이해해서 나중에 필요할때 사용할 수 있게끔 공부했다.

### FCollisionQueryParams

- 사실상 이 로직의 핵심
- 여기서 맞은 물체정보를 가져오는 bReturnPhysicalMaterial을 true로 두어서 PhysicalMaterial의 정보를 가져올 수 있게끔 설정해주었다.
  - HitResult에 재질정보가 같이 옴
  - 나중에 물체의 스테틱메시나 머티리얼에서 Physical Material을 알맞게 설정해준다.
- 또, bTraceComplex를 true로 주어서 복잡한 콜리전을 체크할 수 있도록 하였다.

#### bTraceComplex

- 나중에 알아보니 bTraceComplex는 발소리같이 거의 항상 체크하는 부분에서는 사용하면 안된다고 한다.
  - 성능을 굉장히 많이 잡아먹는다고 한다.
- 또 발소리는 그렇게까지 정교하게 판정을 볼필요가 없기때문에 bTraceComplex는 조금 과하다고 생각했다.
- 이게 false면 그냥 단순히 simple collision만 가지고 판단을 하는데 이게 true면 실제 3D모델의 삼각형 폴리곤을 하나하나 다 직접 보면서 판단해서 cpu부하가 크다고 한다.
  - 공격 판정이나 이런곳에서 사용한다고 한다.
  - 그래서 이를 false로 주고 해결을 해야했다.
- 이를 true로 준이유는 특정 바닥에서 Physical Material을 Metal로 주어도 Metal소리가 안나고 그냥 Default소리가 났다.
  - 오류모음 1에서 수정

### 애니메이션 시퀀스에 지정해주기

- 애니메이션 시퀀스에서 발이 닿을때 AN을 추가해주었다.
- 원래 이것을 Animaition Modifier를 사용해서 해보려고 했는데 안되어서 시간만 잡아먹고 결국에는 수동으로 모두 추가해주었다.

## BP_GCN_FootStep

- GCN은 블루프린트로 설정해주었다.
- 우선 GameplayCue에서 Tag를 AN에서 Foot Step Cue Tag로 지정해주었던 태그를 지정해주었다.

![](https://velog.velcdn.com/images/kyu_/post/1394b424-fda2-4dd9-a5cf-267b1b2c9245/image.png)

- 노드는 대략 이러한데 그냥 바닥 재질정보를 가져와서 재질에 따라 다른 사운드를 출력해주었다.
- 사운드의 경우 발소리가 Default 5종류, Inside(내부발소리) 5종류였는데 이들을 섞어서 사용하기 위해서 simple cue를 만들어서 Play Sound at Location에 지정해주었다.
  - 발소리를 하나씩만 사용하니까 너무 반복되는 소리여서 이질감이 들었다.

# 오류

## 1. 콜리전 오류

- bTraceComplex는 발소리에 사용하는 용이 아니라서 false를 주었는데 특정 바닥에서 소리가 나지 않았다.
- Static Mesh의 Simple Collision Physical Material을 지정해주어도 소리가 나지 않았다.

### 해결 방법

![](https://velog.velcdn.com/images/kyu_/post/d0a6aadb-c193-4725-991a-5f2e42017d3d/image.png)

- show에서 simple collision을 체크하고 확인해보니
- 해당 메시에 simple collision이 설정되어 있지 않았다.

![](https://velog.velcdn.com/images/kyu_/post/1019f70f-f629-44cf-851e-e409779b49e9/image.png)

- Collision 탭에서 add Box Simplified Collison을 선택해서 해결하였다.

![](https://velog.velcdn.com/images/kyu_/post/b44a2de9-ca33-47b3-89f1-9ede0391d2c5/image.png)

![](https://velog.velcdn.com/images/kyu_/post/598e512a-fa70-46a1-9e47-43e0802bc7a4/image.png)

- 삼각형 메시의 경우 10DOP-Z를 통해서 알맞게 설정했는데 다양한 옵션들로 알맞은 콜리전 지정하면 될듯
