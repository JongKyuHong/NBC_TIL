# 언리얼 C++강의 복습

## 캐릭터 클래스에 액션 바인딩 추가

### SetupPlayerInputComponent

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
- SetupPlayerInputComponent은 그냥 `연결`만 한다.
  - 우리가 PlayerController에다가 만들어놓은 SprintAction과 같은 액션들과 그에 맞게 동작하는 함수를 연결함
  - PlayerController에서 UPROPERTY를 썼기때문에 에디터에 액션들이 보이고 그 액션에 IA를 맞춰서 넣으면 된다.

### Move

```c++
void ATestCharacter::Move(const FInputActionValue& value)
{
	if (!Controller) return;
}
```

- Controller는 APawn클래스에 선언되어있다. 컨트롤러 가져오는것

### Sprint

```c++
void ATestCharacter::StartSprint(const FInputActionValue& value)
{
	if (GetCharacterMovement())
	{
		GetCharacterMovement()->MaxWalkSpeed = SprintSpeed;
	}
}

void ATestCharacter::StopSprint(const FInputActionValue& value)
{
	if (GetCharacterMovement())
	{
		GetCharacterMovement()->MaxWalkSpeed = NormalSpeed;
	}
}
```

- GetCharacterMovement()는 ACharacter에 기본으로 들어있는 함수

## 2-3 숙제

### 1. 점프 횟수 세기

```c++
if (PlayerController->JumpAction)
{
	EnhancedInput->BindAction(
		PlayerController->JumpAction,
		ETriggerEvent::Started,
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

void ATestCharacter::StartJump(const FInputActionValue& value)
{
	if (CanJump()) {
		Jump();
		JumpCount++;
		UE_LOG(LogTemp, Warning, TEXT("Jump Count : %d"), JumpCount);
	}

}

void ATestCharacter::StopJump(const FInputActionValue& value)
{
	StopJumping();
}
```

- 점프를 Trigger에서 Started로 바꾸고 CanJump()를 추가하여 공중에서 스페이스눌렀을때 세던것도 고쳤다.

### 2. 스프린트 속도 동적 조정

```c++
void ATestCharacter::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
	SprintSpeed = NormalSpeed * SprintSpeedMultiplier;
}
```

- OnConstruction : 에디터에서 속성 변경할때마다 적용

## 3-2강

```c++
ABaseItem::ABaseItem()
{
    PrimaryActorTick.bCanEverTick = false;

    // 루트 컴포넌트 생성 및 설정
    Scene = CreateDefaultSubobject<USceneComponent>(TEXT("Scene"));
    SetRootComponent(Scene);

    // 충돌 컴포넌트 생성 및 설정
    Collision = CreateDefaultSubobject<USphereComponent>(TEXT("Collision"));
    // 겹침만 감지하는 프로파일 설정
    Collision->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
    // 루트 컴포넌트로 설정
    Collision->SetupAttachment(Scene);

    // 스태틱 메시 컴포넌트 생성 및 설정
    StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
    StaticMesh->SetupAttachment(Collision);
		// 메시가 불필요하게 충돌을 막지 않도록 하기 위해, 별도로 NoCollision 등으로 설정할 수 있음.

    // Overlap 이벤트 바인딩
    Collision->OnComponentBeginOverlap.AddDynamic(this, &ABaseItem::OnItemOverlap);
    Collision->OnComponentEndOverlap.AddDynamic(this, &ABaseItem::OnItemEndOverlap);
}
```

- 루트 컴포넌트를 생성하고, 충돌 컴포넌트를 생성한다.
- 충돌 프로파일을 설정하고 루트컴포넌트에 달아주고, 스태틱메시도 달아줌
- OnComponentBeginOverlap과 OnItemOverlap을 연결
- 마찬가지로 OnComponentEndOverlap과 OnItemEndOverlap 연결

## 3-3강

### 랜덤 위치 생성

```c++
FVector BoxExtent = SpawningBox->GetScaledBoxExtent();  // 박스의 반지름 (X,Y,Z)
FVector BoxOrigin = SpawningBox->GetComponentLocation(); // 박스 중심점

return BoxOrigin + FVector(
    FMath::FRandRange(-BoxExtent.X, BoxExtent.X),  // X축 랜덤
    FMath::FRandRange(-BoxExtent.Y, BoxExtent.Y),  // Y축 랜덤
    FMath::FRandRange(-BoxExtent.Z, BoxExtent.Z)   // Z축 랜덤
);
```

### 아이템 스폰

```c++
void ASpawnVolume::SpawnItem(TSubclassOf<AActor> ItemClass)
{
    if (!ItemClass) return;  // 안전장치

    GetWorld()->SpawnActor<AActor>(
        ItemClass,           // 어떤 클래스? (KeyItem, Coin 등)
        GetRandomPointInVolume(),  // 어디에?
        FRotator::ZeroRotator     // 회전은 기본값 (0,0,0)
    );
}
```
