![](https://velog.velcdn.com/images/kyu_/post/af4c499b-5bda-404d-bb8f-a58d9ae02511/image.png)

# 복습

## 매핑

```c++
if (UEnhancedInputComponent* EnhancedInput = Cast<UEnhancedInputComponent>(PlayerInputComponent))
{
	if (AMyPlayerController* PlayerController = Cast<AMyPlayerController>(GetController()))
	{
		if (PlayerController->MoveAction)
		{
			EnhancedInput->BindAction(
				PlayerController->MoveAction,
				ETriggerEvent::Triggered,
				this,
				&AMyCharacter::Move
			);
		}
    }
}
```

- PlayerInputComponent를 향상된 인풋 시스템으로 캐스팅함
- GetController로 플레이어 컨트롤러를 가져옴
- PlayerController의 액션과 함수를 매핑

## 이동

```c++
const FVector2D MoveInput = value.Get<FVector2D>();

if (!FMath::IsNearlyZero(MoveInput.X))
{
	AddMovementInput(GetActorForwardVector(), MoveInput.X);
}

if (!FMath::IsNearlyZero(MoveInput.Y))
{
	AddMovementInput(GetActorRightVector(), MoveInput.Y);
}
```

## Look

```c++
FVector2D LookInput = value.Get<FVector2D>();

AddControllerYawInput(LookInput.X);
AddControllerPitchInput(LookInput.Y);
```

- AddControllerYawInput, AddControllerPitchInput으로 시점 변경
- AddMovementInput으로 이동

## Sprint

```c++
if (GetCharacterMovement())
{
	GetCharacterMovement()->MaxWalkSpeed = SprintSpeed;
}
```

-CharacterMovementComponent는 캐릭터가 공중에 떠 있는지, 바닥에 닿아 있는지, 현재 속도는 얼마인지 등을 계산하는 아주 복잡한 수학적 로직이 들어있는 컴포넌트

# 게임수학

## 삼각함수

### sin함수

- 기준각과 마주보는 대변/빗변
- 비율

![](https://velog.velcdn.com/images/kyu_/post/a632e22f-61be-4860-980e-9a67a38a4a75/image.png)

### cos함수

- 인접변/빗변
- 사인에 협업하는 co-sin이라고 기억하면 좋다.

### tan함수

- 인접변/대변
- 기울기라는 개념으로 쓰일 수 있다.
- 탑다운 방식에서 캐릭터가 어디를 바라보느냐에 쓰인다.

### 역삼각함수

- arcsin
- arccos
- arctan
- 원래 y = sinθ 였다면 역삼각함수에서는 θ = arcsin y, θ = sin\*\*-1 y

## 삼각함수 덧셈

### 제1코사인법칙

- a = b cos C + c cos B
- b = a cos C + c cos A
- c = a cos B + b cos A

![](https://velog.velcdn.com/images/kyu_/post/dec8e16b-77b5-4df0-bf23-7adf76f00ff7/image.png)

### 벡터의 내적

내적은 벡터의 곱셈, 하지만 값은 스칼라

![](https://velog.velcdn.com/images/kyu_/post/ae3a40fd-daf8-4f2a-af64-443cc3e8750b/image.png)

- 쉐이더에 사용

### 덧셈 정리

![](https://velog.velcdn.com/images/kyu_/post/a79ae17d-7026-4130-b4c1-08b401f5a359/image.png)

![](https://velog.velcdn.com/images/kyu_/post/c17d58be-5230-4faf-94b1-d3c269622e35/image.png)

## 행렬의 곱셈

![](https://velog.velcdn.com/images/kyu_/post/df409cd0-8db1-4110-8fb5-cfdf481db641/image.png)

- 행렬의 곱셈도 어떻게 보면 내적이라고 볼 수 있다.
- 앞행렬의 열의 개수와 뒤행렬의 행의 개수가 같아야 곱할 수 있다.
- 스케일, 회전, 회전+이동 모두 행렬로 표현할 수 있다.
- 회전+이동을 사용하려면 한차원을 늘려야함 그래서 4차원행렬이 나오는 것

## 직교행렬

- x, y, z축 벡터들이 서로 직각을 이루는 행렬
- 모든 축 벡터가 단위벡터, 역행렬을 하면 전치행렬(행과 열만 바뀜)
- 회전과 방향에 사용한다.
- 만약 스케일을 늘렸다면 더이상 직교행렬이 아니다. 무조건 길이가 1이어야함

## 정방행렬

- 행의 개수와 열의 개수가 같음

## 단위행렬

![](https://velog.velcdn.com/images/kyu_/post/28109499-3439-4a88-b124-41535338b7dd/image.png)

## Transform

- 행렬과 벡터의 곱은 Transform을 의미한다.

![](https://velog.velcdn.com/images/kyu_/post/6ce71ea7-adf0-449d-80ff-89c414f95cf3/image.png)
