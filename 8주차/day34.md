# 언리얼 C++강의 복습

## 캐릭터 클래스에 액션 바인딩 추가

- 입력 바인딩을 처리해줄 함수를 선언한다.
  - SetupPlayerInputComponent

```c++
virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
```

- Input Action을 처리할 함수를 만든다.
  - Enhanced Input에서 액션값은 FInputActionValue로 전달된다.

```c++
UFUNCTION()
void Move(const FInputActionValue& value);
```

- SetupPlayerInputComponent 함수 만들기
  - 사용자 입력에 따라 알맞은 함수 바인딩해주기
  - PlayerInputController를 향상된 인풋으로 캐스팅하고
  - GetController()를 통해 현재 이 캐릭터를 조종하는 Controller를 가져와서 우리가 만든 컨트롤러에 캐스팅

```c++
void ATestCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInput = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		if (ATestPlayerController* PlayerController = Cast<ATestPlayerController>(GetController()))
		{
			if (PlayerController->MoveAction)
			{
				EnhancedInput->BindAction(
					PlayerController->MoveAction,
					ETriggerEvent::Triggered,
					this,
					&ATestCharacter::Move
				);
			}

			if (PlayerController->JumpAction)
			{
				EnhancedInput->BindAction(
					PlayerController->JumpAction,
					ETriggerEvent::Triggered,
					this,
					&ATestCharacter::StartJump
				);

				EnhancedInput->BindAction(
					PlayerController->JumpAction,
					ETriggerEvent::Completed,
					this,
					&ATestCharacter::StopJump
				);
			}

			if (PlayerController->LookAction)
			{
				EnhancedInput->BindAction(
					PlayerController->LookAction,
					ETriggerEvent::Triggered,
					this,
					&ATestCharacter::Look
				);
			}

			if (PlayerController->SprintAction)
			{
				EnhancedInput->BindAction(
					PlayerController->SprintAction,
					ETriggerEvent::Triggered,
					this,
					&ATestCharacter::StartSprint
				);
				EnhancedInput->BindAction(
					PlayerController->SprintAction,
					ETriggerEvent::Completed,
					this,
					&ATestCharacter::StopSprint
				);
			}
		}
	}
}

```

- 바인딩되는 함수들 만들기

```c++
void ATestCharacter::Move(const FInputActionValue& value)
{
	if (!Controller) return;
}
```

- Controller는 APawn클래스에 선언되어있다. 컨트롤러 가져오는것
