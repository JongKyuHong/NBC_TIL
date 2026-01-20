# 7번과제

## Tick

- Line Trace
- 눈에 보이지않는 레이저 포인터를 쏘는 것
  - 이 레이저가 무언가(벽, 바닥 등)에 부딪히면 그 지점의 정보(좌표, 거리, 부딪힌 물체 등)를 가져온다.

- 시작점, 끝점, 검사(시작점과 끝점 사이에 충돌체가 있는지)

```c++
void AFlyingPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FHitResult HitResult;
	FVector Start = GetActorLocation();
	FVector End = Start + (FVector::DownVector * TraceDistance);

	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this);

	bIsGrounded = GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, ECC_Visibility, Params);

	if (!bIsGrounded)
	{
		VerticalVelocity += GravityStrength * DeltaTime;
	} else
	{
		if (VerticalVelocity < 0.0f)
		{
			VerticalVelocity = 0.0f;
		}

		float CapsuleHalfHeight = RootComp->GetScaledCapsuleHalfHeight();
		float TargetZ = HitResult.Location.Z + CapsuleHalfHeight;

		FVector CurrentLocation = GetActorLocation();
		if (CurrentLocation.Z < TargetZ)
		{
			CurrentLocation.Z = TargetZ;
			SetActorLocation(CurrentLocation);
		}
	}
	AddActorWorldOffset(FVector(0.0f, 0.0f, VerticalVelocity * DeltaTime), true);
}

```

- DownVector는 (0, 0, -1)을 가진 방향 벡터 그래서 DownVector에 TraceDistance만큼의 거리를 곱해서 바닥이랑 얼마나 떨어져있는지를 Tick마다 확인한다.
- Params.AddIgnoredActor(this);
  - 본인은 Line Trace에서 제외

- bIsGrounded = GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, ECC_Visibility, Params);
  - 실제로 레이저 발사
  - ECC_Visibility는 언리얼에 기본적으로 있는 충돌 채널
  - ECC_WorldStatic : 움직이는 배경 물체 전용
  - ECC_Pawn : 플레이어나 몬스터 같은 캐릭터 전용
  - ECC_Visibility : 마우스 클릭, 레이저 포인터 등 `보이는 물체`를 대상으로 할때 전용
  - 만약에 부딪히는게 있다? bIsGround를 true로 해서 바닥이다! 판단
- VerticalVelocity : 수직으로 땅에 떨어지는 속도, 중력가중치와 DeltaTime을 곱해서 시간이 지날수록 빠르게 떨어지게끔

- float CapsuleHalfHeight = RootComp->GetScaledCapsuleHalfHeight();
- float TargetZ = HitResult.Location.Z + CapsuleHalfHeight;
  - 위 두코드는 비행체가 바닥쪽으로 돌아가 있을때 Space를 누르게되면 바닥을 뚫어버리는 문제가있었는데 HitResult가 바닥이면 거기에 충돌을 담당하는 Capsule의 절반만큼의 높이를 더해서 CurrentLocation.Z < TargetZ를 통해 바닥을 뚫지 않게끔 만들어주었다.

## MoveUp, Space/Left Shift

```c++
void AFlyingPawn::MoveUp(const FInputActionValue& Value)
{
	float UpDownValue = Value.Get<float>();
	float DeltaTime = GetWorld()->GetDeltaSeconds();

	if (!FMath::IsNearlyZero(UpDownValue))
	{
		float ThrustStrength = 1500.0f;

		if (UpDownValue > 0.0f) // 상승 시 SPace눌렀을때
		{
			VerticalVelocity += ThrustStrength * DeltaTime;

			VerticalVelocity = FMath::Clamp(VerticalVelocity, -2000.f, 1500.f);
		} else // Shift누를때
		{
			VerticalVelocity -= ThrustStrength * DeltaTime;
		}
		AddActorLocalOffset(FVector(0.0f, 0.0f, UpDownValue * MoveSpeed * DeltaTime), true);
	}
}
```

- Space를 누르면 상승 ThrustStrength 추진력을 이용해서 VerticalVelocty에 더함 (나중에 Tick에서 중력과 함께 계산됨)
- 하강 속도는 -2000, 상승은 1500으로 제한
