![](https://velog.velcdn.com/images/kyu_/post/4047e0c0-29bb-47c3-9436-1a2ca9f71c44/image.png)
![](https://velog.velcdn.com/images/kyu_/post/1e3a8716-6045-490a-ad02-d03e1f69c38e/image.png)
~~커엽~~

# 8번 과제

## 2. GAS

- [참고](https://dev.epicgames.com/documentation/ko-kr/unreal-engine/gameplay-ability-system-for-unreal-engine)

### AttributeSet 작성하기

```c++
#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "AbilitySystemComponent.h"
#include "MyAttributeSet.generated.h"


#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)

UCLASS()
class ASSIGNMENT8_API UMyAttributeSet : public UAttributeSet
{
	GENERATED_BODY()
public:
	UMyAttributeSet();

	UPROPERTY(BlueprintReadOnly, Category = "Attributes")
	FGameplayAttributeData Health;
	ATTRIBUTE_ACCESSORS(UMyAttributeSet, Health)

	UPROPERTY(BlueprintReadOnly, Category = "Attributes")
	FGameplayAttributeData MaxHealth;
	ATTRIBUTE_ACCESSORS(UMyAttributeSet, MaxHealth)

	UPROPERTY(BlueprintReadOnly, Category = "Attributes")
	FGameplayAttributeData MoveSpeed;
	ATTRIBUTE_ACCESSORS(UMyAttributeSet, MoveSpeed)

	UPROPERTY(BlueprintReadOnly, Category = "Attributes")
	FGameplayAttributeData JumpZVelocity;
	ATTRIBUTE_ACCESSORS(UMyAttributeSet, JumpZVelocity)

	UPROPERTY(BlueprintReadOnly, Category = "Attributes")
	FGameplayAttributeData CharacterScale;
	ATTRIBUTE_ACCESSORS(UMyAttributeSet, CharacterScale)

	UPROPERTY(BlueprintReadOnly, Category = "Attributes")
	FGameplayAttributeData ExpGainRate;
	ATTRIBUTE_ACCESSORS(UMyAttributeSet, ExpGainRate)

	virtual void PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data) override;
};
```

- AttributeSet으로 사용할 Attribute를 모아준다.

### 캐릭터에 등록하기

- InitAbilityActorInfo를 꼭 써주어야 ASC를 사용할 수 있는데 이는 C++전용함수
- 캐릭터의 Beginplay에 작성
- 캐릭터헤더에 ASC초기화 AttributeSet관리를 위한 함수,변수 추가

```c++
// MyChatacter.h
#include "AbilitySystemInterface.h"

class UAbilitySystemComponent;
class UMyAttributeSet;

virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override { return AbilitySystemComponent; }

UPROPERTY()
TObjectPtr<UMyAttributeSet> AttributeSet;

UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "GAS")
class UAbilitySystemComponent* AbilitySystemComponent;

// MyCharacter.cpp
AMyCharacter::AMyCharacter()
{
  //...기존로직...
AbilitySystemComponent = CreateDefaultSubobject<UAbilitySystemComponent>(TEXT("AbilitySystemComponent"));
AttributeSet = CreateDefaultSubobject<UMyAttributeSet>(TEXT("AttributeSet"));
}

void AMyCharacter::BeginPlay()
{
	Super::BeginPlay();
	if (AbilitySystemComponent)
	{
		AbilitySystemComponent->InitAbilityActorInfo(this, this);
	}
}
```

## 3. 데이터 에셋

### PerkDataAsset.h

```c++
UENUM(BlueprintType)
enum class EPerkRarity : uint8
{
	Silver, // 일반
	Gold,  // 희귀
	Prism  // 전설 느낌?, 보라~프리즘색
};

// 하나의 Perk이 가져야 하는 정보
USTRUCT(BlueprintType)
struct FPerkInfo
{
	GENERATED_BODY()

	// Perk이름
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText PerkNames;

	// 이 Perk과 연동된 Gameplay Effect클래스
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<class UGameplayEffect> PerkEffectClass;
};

// 무작위로 3개의 특성을 가져옴 / 화면에 전달되는 단위
USTRUCT(BlueprintType)
struct FPerkRollResult
{
	GENERATED_BODY()

	// 선택된 3개의 퍽들
	UPROPERTY(BlueprintReadWrite)
	TArray<FPerkInfo> SelectedPerks;
};

UCLASS()
class ASSIGNMENT8_API UPerkDataAsset : public UPrimaryDataAsset
{
	GENERATED_BODY()
public:
	// 모든 Perk들
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Perks")
	TArray<FPerkInfo> AllPerks;
};
```

- Perk에대한 정보를 담은 데이터 에셋

### PerkManagerComponent

```c++
// meta=BlueprintSpawnableComponent 블루프린트에서 사용할 수 있게
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ASSIGNMENT8_API UPerkManagerComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UPerkManagerComponent();

	// 3개의 퍽을 랜덤으로 정하는 함수
	FPerkRollResult RollPerkOptions(UPerkDataAsset* DataAsset);
};
```

- 퍽 계산을 위한 함수
- 현재는 로직이 많이 바뀌어서 함수가 하나밖에없다.

## 4. 레벨업 만들기

### MyAttributeSet.h

```c++
UPROPERTY(BlueprintReadOnly, Category = "Attributes")
FGameplayAttributeData Level;
ATTRIBUTE_ACCESSORS(UMyAttributeSet, Level)

UPROPERTY(BlueprintReadOnly, Category = "Attributes")
FGameplayAttributeData Exp;
ATTRIBUTE_ACCESSORS(UMyAttributeSet, Exp)

UPROPERTY(BlueprintReadOnly, Category = "Attributes")
FGameplayAttributeData MaxExp;
ATTRIBUTE_ACCESSORS(UMyAttributeSet, MaxExp)
```

- 캐릭터 레벨, 경험치, 요구경험치를 추가했다.

### MyAttributeSet.cpp

```c++
if (ModifiedAttribute == GetExpAttribute())
{
  float CurrentExp = GetExp();
  float MaxExpValue = GetMaxExp();

  if (MaxExpValue <= 0.f) return;

  if (CurrentExp >= MaxExpValue)
  {
    SetExp(CurrentExp - MaxExpValue);
    SetLevel(GetLevel() + 1.f);
    SetMaxExp(GetLevel() * 100.f);
    float RemainingExp = GetExp() - GetMaxExp();
    float NewLevel = GetLevel() + 1.0f;
    SetLevel(NewLevel);
    SetMaxExp(NewLevel * 100.0f);
    SetExp(RemainingExp);
    AMyGameState* MyGameState = GetWorld() ? GetWorld()->GetGameState<AMyGameState>() : nullptr;
    if (MyGameState)
    {
      MyGameState->ShowPerkMenu();
    }
    UE_LOG(LogTemp, Warning, TEXT("Level Up! Current Level: %d"), (int32)NewLevel);
  }

  SetExp(FMath::Max(GetExp(), 0.0f));
}

```

- cpp에서도 레벨업 로직을 넣어놓았다.

### CoinItem.cpp

```c++
void ACoinItem::ActivateItem(AActor* Activator)
{
	Super::ActivateItem(Activator);
  // 동전과 오버랩된 Activator에게 ASC가 있는지
  UAbilitySystemComponent* ASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(Activator);


  if (ASC && ExpEffectClass)
  {
      // 경험치율 가져오기, 매크로덕에 Get으로 AttributeSet에서 가져올 수 있음
      float GainRate = ASC->GetNumericAttribute(UMyAttributeSet::GetExpGainRateAttribute());

      // 경험치율 적용
      float FinalExp = ExpValue * GainRate;

      // 에디터에서 설정한 ExpEffectClass를 바탕으로 실제 적용할 데이터 Spec만들기
      FGameplayEffectSpecHandle SpecHandle = ASC->MakeOutgoingSpec(ExpEffectClass, 1.0f, ASC->MakeEffectContext());
      if (SpecHandle.IsValid())
      {
          // C++에서 계산한 경험치를 GE에 넣음
          SpecHandle.Data.Get()->SetSetByCallerMagnitude(
              FGameplayTag::RequestGameplayTag(FName("Data.Exp")),
              FinalExp
          );
          // 캐릭터에도 적용시켜달라고 ASC에게 명령
          ASC->ApplyGameplayEffectSpecToTarget(*SpecHandle.Data.Get(), ASC);
      }
  }
  else if (!ExpEffectClass)
  {
      UE_LOG(LogTemp, Error, TEXT("ExpEffectClass 없음"));
  }
  // ... 기존로직...
}
```

- 작은 코인은 경험치 10, 큰거는 20으로 설정했다.
- ExpEffectClass를 만들고 경험치 획득

## 5. 웨이브 만들기

### 웨이브 관리

```c++
void AMyGameState::StartLevel()
{
  // ...기존로직...
  StartWave();
}
```

- 레벨이 시작하면 StartWave() 함수 시작
- StartWave()함수는 그냥 스폰해주고 타이머달아주고 강의에서듣던 startLevel과 같은역할

```c++
void AMyGameState::OnWaveTimeUp()
{
    EndWave();
}

void AMyGameState::EndWave()
{
    GetWorldTimerManager().ClearTimer(WaveTimerHandle);
    CurrentWaveIndex++;
    if (CurrentWaveIndex >= MaxWaves)
    {
        EndLevel();
    }
    else
    {
        StartWave();
    }
}
```

- 웨이브 타이머가 끝나면 OnWaveTiemUp 소환
- EndWave()는 웨이브가 3단계끝났다면 다음레벨로

### UI띄우는 부분

```c++
void AMyGameState::ShowPerkMenu() const
{
    if (APlayerController* PlayerController = GetWorld()->GetFirstPlayerController())
    {
        if (AMyPlayerController* MyPlayerController = Cast<AMyPlayerController>(PlayerController))
        {
            MyPlayerController->ShowPerkUI();
        }
    }
}

void AMyPlayerController::ShowPerkUI()
{
    if (PerkWidgetClass && MyPerkDataAsset)
    {
        FPerkRollResult Options = PerkManager->RollPerkOptions(MyPerkDataAsset);

        PerkWidgetInstance = CreateWidget<UUserWidget>(this, PerkWidgetClass);
        if (PerkWidgetInstance)
        {
            PerkWidgetInstance->AddToViewport();

            FName FuncName = FName("UpdatePerkDisplay");
            UFunction* UpdateFunc = PerkWidgetInstance->FindFunction(FuncName);
            if (UpdateFunc)
            {
                PerkWidgetInstance->ProcessEvent(UpdateFunc, &Options);
            }

            SetInputMode(FInputModeUIOnly());
            bShowMouseCursor = true;
            //SetPause(true);
        }
    }
}
```

- Perk을 띄우는 UI를 보여주는 함수
- 약간 증바람의 증강고르는것처럼 연출하려고 한다.

```c++
void AMyPlayerController::ShowGameHUD()
{
  if (PerkWidgetInstance)
{
    // SetPause(false);
    PerkWidgetInstance->RemoveFromParent();
    PerkWidgetInstance = nullptr;
}
}
```

- GameHUD가 보일때는 Perk을 해제시켜주었다.

### EndLevel

- 3웨이브를 다 클리어하면 레벨이 끝남

```c++
void AMyGameState::EndLevel()
{
  // ... 기존로직 ...
  if (AttributeSet)
  {
      MyGameInstance->Health = AttributeSet->GetHealth();
      MyGameInstance->MaxHealth = AttributeSet->GetMaxHealth();
      MyGameInstance->MoveSpeed = AttributeSet->GetMoveSpeed();
      MyGameInstance->JumpZVelocity = AttributeSet->GetJumpZVelocity();
      MyGameInstance->CharacterScale = AttributeSet->GetCharacterScale();
      MyGameInstance->ExpGainRate = AttributeSet->GetExpGainRate();
      MyGameInstance->Level = AttributeSet->GetLevel();
      MyGameInstance->Exp = AttributeSet->GetExp();
      MyGameInstance->MaxExp = AttributeSet->GetMaxExp();
  }
  // ... 기존로직 ...
}
```

- 레벨이 끝나면 GameInstance에 정보들을 담아줌
- 증강을 통해서 이동속도나 Scale정보가 바뀔수있는데 다음 레벨에서 유지하려면 인스턴스를 쓸수밖에없었다.

```c++
UCLASS()
class ASSIGNMENT8_API UMyGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	UMyGameInstance();

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "GameData")
	int32 TotalScore;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "GameData")
	int32 CurrentLevelIndex;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "GameData")
	float Health;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "GameData")
	float MaxHealth;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "GameData")
	float MoveSpeed;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "GameData")
	float JumpZVelocity;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "GameData")
	float CharacterScale;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "GameData")
	float ExpGainRate;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "GameData")
	float Level;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "GameData")
	float Exp;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "GameData")
	float MaxExp;

	UFUNCTION(BlueprintCallable, Category = "GameData")
	void AddToScore(int32 Amount);
};

```

- 인스턴스에도 추가

### 블루프린트

![](https://velog.velcdn.com/images/kyu_/post/20e5e703-4861-4562-9eef-73901888f54d/image.png)

- 증강 등급 뽑기

![](https://velog.velcdn.com/images/kyu_/post/846aadb8-c5b3-4788-b07e-32b9b937facf/image.png)

- 증강 등급에 맞는 가중치 부여해서 UI로 띄워줌

![](https://velog.velcdn.com/images/kyu_/post/bccff714-2b51-4a81-8077-664bbf64ef8e/image.png)

- 게임 이펙트 생성
- 가중치 계산
- 실제 적용

### 결과물

![](https://velog.velcdn.com/images/kyu_/post/2133dddd-7c88-4aae-a807-2a8a1eb3720b/image.png)

- 여기에 등급별 색상이랑 이미지 부여하기

# 트러블 슈팅

## 1. 맵 레벨 넘어갈때 크래시

```
void AMyGameState::EndLevel()
{
    if (UGameInstance* GameInstance = GetGameInstance())
    {
        UMyGameInstance* MyGameInstance = Cast<UMyGameInstance>(GameInstance);
        if (MyGameInstance)
        {
            AddScore(Score);
            CurrentLevelIndex++;
            MyGameInstance->CurrentLevelIndex = CurrentLevelIndex;

            if (APlayerController* PlayerController = GetWorld()->GetFirstPlayerController())
            {
                if (APawn* PlayerPawn = PlayerController->GetPawn())
                {
                    if (UAbilitySystemComponent* ASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(PlayerPawn))
                    {
                        const UMyAttributeSet* AttributeSet = Cast<UMyAttributeSet>(ASC->GetAttributeSet(UMyAttributeSet::StaticClass()));
                        if (AttributeSet)
                        {
                            MyGameInstance->Health = AttributeSet->GetHealth();
                            MyGameInstance->MaxHealth = AttributeSet->GetMaxHealth();
                            MyGameInstance->MoveSpeed = AttributeSet->GetMoveSpeed();
                            MyGameInstance->JumpZVelocity = AttributeSet->GetJumpZVelocity();
                            MyGameInstance->CharacterScale = AttributeSet->GetCharacterScale();
                            MyGameInstance->ExpGainRate = AttributeSet->GetExpGainRate();
                            MyGameInstance->Level = AttributeSet->GetLevel();
                            MyGameInstance->Exp = AttributeSet->GetExp();
                            MyGameInstance->MaxExp = AttributeSet->GetMaxExp();
                        }
                    }
                }
            }
            if (CurrentLevelIndex >= MaxLevels)
            {
                OnGameOver();
                return;
            }

            if (LevelMapNames.IsValidIndex(CurrentLevelIndex))
            {
                UGameplayStatics::OpenLevel(GetWorld(), LevelMapNames[CurrentLevelIndex]);
            } else
            {
                OnGameOver();
            }
        }
    }
}
```

- 기존에는 GameState에서 AttributeSet을 불러와서 사용을 했는데 그렇게 하지 않고 Player에서 가져오고 방어코드 if (AttributeSet)을 달아주니 해결되었다.

## 2. 큰동전은 경험치가 안오름

- 1번문제를 해결하니 갑자기 생김

```c++
// CoinItem.h
UPROPERTY(EditAnywhere, Category = "GAS")
TSubclassOf<class UGameplayEffect> ExpEffectClass;
```

- CoinItem에서 정의했던 ExpEffectClass가 BigCoin에서 지정되어있지 않았다.
- 에디터 오류인듯
