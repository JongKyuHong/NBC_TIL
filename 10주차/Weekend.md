# 트러블 슈팅

## 3. 캐릭터 레벨

- 맵 레벨이 넘어갈때 캐릭터 스텟정보르 제대로 못불러옴
- GameInstance에 저장할때 제대로 안된것같다.
- 오늘은 해결 못할듯?
- 그냥 StartLevel에서 적용을 안해서 그랬다.

## 4. 증강 등장시 키 입력

```c++
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
            SetIgnoreMoveInput(true);
            SetIgnoreLookInput(true);
            bShowMouseCursor = true;
            //SetPause(true);
        }
    }
}
```

- 증강이 등장했을때 앞으로 뛰어가던걸 해당 코드로 막고있었다.
- 추가적인 입력을 막을뿐 점프는 막을 수 없었다.

```c++
APawn* MyPawn = GetPawn();
if (!MyPawn) return;
ACharacter* MyCharacter = Cast<ACharacter>(MyPawn);
if (!MyCharacter) return;
if (UCharacterMovementComponent* MoveComp = MyCharacter->GetCharacterMovement())
{
    MoveComp->StopMovementImmediately();
    MoveComp->DisableMovement();
}
```

- 아예 입력자체를 다 막는 코드를 고안했으나 게임 멈추는게 더 낫다고 판단이되어서 setPause를 다시 사용하기로 했다.

## 5. 스텟 적용안됨

```c++
CurrentLevelIndex = MyGameInstance->CurrentLevelIndex;
if (CurrentLevelIndex == 0)
{
    MyGameInstance->ResetAllStats();
}
if (APawn* PlayerPawn = PlayerController->GetPawn())
{
    if (UAbilitySystemComponent* ASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(PlayerPawn))
    {
        UMyAttributeSet* AS = const_cast<UMyAttributeSet*>(Cast<UMyAttributeSet>(ASC->GetAttributeSet(UMyAttributeSet::StaticClass())));

        if (AS)
        {
            AS->SetHealth(MyGameInstance->Health);
            AS->SetMaxHealth(MyGameInstance->MaxHealth);
            AS->SetMoveSpeed(MyGameInstance->MoveSpeed);
            AS->SetJumpZVelocity(MyGameInstance->JumpZVelocity);
            AS->SetCharacterScale(MyGameInstance->CharacterScale);
            AS->SetExpGainRate(MyGameInstance->ExpGainRate);
            AS->SetLevel(MyGameInstance->Level);
            AS->SetExp(MyGameInstance->Exp);
            AS->SetMaxExp(MyGameInstance->MaxExp);
        }
    }
}
```

- CurrentLevel이 0일때만 초기상태를 불러오도록 조정하였다.

```c++
void UMyGameInstance::ResetAllStats()
{
    TotalScore = 0;
    CurrentLevelIndex = 0;
    Health = 100.0f;
    MaxHealth = 100.0f;
    MoveSpeed = 400.0f;
    CharacterScale = 1.0f;
    ExpGainRate = 1.0f;
    Level = 1.0f;
    Exp = 0.0f;
    MaxExp = 100.0f;
}
```

- GameInstance에도 초기값을 넣은 함수를 추가해주었다.
