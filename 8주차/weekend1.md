# 6번과제하기

## 요구사항 해석

```
간단한 퍼즐용 오브젝트를 준비하는 과제인데 회전 발판, 이동 플랫폼등 동적으로 움직이는 발판을 만들고 이거를 C++로직, Tick함수로 제어하는 과제

최소 2가지 이상의 C++ Actor클래스를 가져야하고, ㄴStaticMeshComponent를 가져야 한데, 이것들은 그냥 발판으로 2가지 이상 만들면 될 것 같다.

Tick함수를 활용해서 회전, 이동 발판을 만들어야 하고 리플렉션 시스템을 이용해서 각각 다른속도, 이동범위, 회전값등을 적용해보고 실험도 해보자

또, 마지막으로 타이머 시스템을 활용해서 일정 시간 후에 발판이 사라지거나 주기걱으로 다른 위치로 이동하는 추가 구현과제랑
SpawnActor를 통해 임의 좌표에 여러개 배치하는 랜덤 스테이지의 기초개념을 공부할 수 있는 추가 구현 과제도 구현해보자
```

## 체크포인트

--- 필수 과제 ---

- [x] 레벨 꾸미기 (시작/골인/대략적인 동선구성)
- [x] GameMode, Default Map지정
- [ ] C++ 클래스 만들기
  - [x] MyCharacter 클래스
    - [x] IA, IMC 만들기
    - [x] Animation만들기
  - [x] BasePlatform 클래스
    - [x] MovingPlatform
    - [x] RotatingPlatform
  - [ ] FinishZone
- [ ] C++ 클래스 구현
  - [x] C++ 클래스를 블루프린트로 상속하기
    - [x] 각 클래스 완성 시 레벨에 배치하고 테스트

--- 도전 과제 ---

- [ ] 타이머를 통해서 발판 사라지고 주기적으로 이동하는거 구현
- [ ] 랜덤하게 발판 배치되는 랜덤퍼즐 생성해보기

--- 과제를 끝내며 ---

- [ ] AI보다는 공식문서를 많이 활용했는가??

### 레벨 꾸미기

![](https://velog.velcdn.com/images/kyu_/post/d29986d5-6767-48ab-9320-3a8c5e80e587/image.png)

- 우선 New level에서 Basic을 선택 한 후 Scale을 4로 줄인 발판 3개를 만들었다.
- 각각 1만씩 떨어져있으며 왼쪽부터 시작, 중간지점, 끝 이렇게 나누었다.
  - 시작 <-> 중간 : 필수과제
  - 중간 <-> 끝 : 도전과제

### GameMode, Default Map지정

![](https://velog.velcdn.com/images/kyu_/post/1d7b92b4-559d-4077-8780-f29f4ebdb5c3/image.png)

- Default Map은 만든 Level로 지정을 해주고, 게임모드는 BP로 상속한 게임모드를 지정
  - 레벨을 여러개 사용하지 않을것이라 전역으로 게임모드를 지정해주기만 했다.

### C++ 클래스 만들기

#### 캐릭터 만들기

- 캐릭터 클래스 생성 + 블루프린트
- IA, IMC만들고 BP_MyPlayerController에 매핑
- LocalPlayerSubsystem을 통해 IMC를 활성화

#### RotatingPlatform 만들기

![](https://velog.velcdn.com/images/kyu_/post/cfe98296-51a1-4e85-9135-e4638a32a382/image.png)

- 스태틱메시로는 별을 선택했고, 콜리전이 없어서 에셋에디터에서 Collision을 Use Complex Collision As Simple로 설정했다.
- 발판이 회전하면서 캐릭터도 같이 돌아주기 때문에 캐릭터에서 GetCharacterMovement()->bIgnoreBaseRotation = true;로 주었다.

#### MovingPlatform 만들기

##### 발판이 한쪽으로만 이동하는 문제

![](https://velog.velcdn.com/images/kyu_/post/d22fe6ed-455f-4c67-8f75-3fd6ac4bf0ba/image.gif)

- 기존 코드

```c++
void AMovingPlatform::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    if (!FMath::IsNearlyZero(MoveSpeed))
    {
        if (GetActorLocation().X > MaxRange) {
            MoveDirection *= -1;
        }
        AddActorLocalOffset(FVector(MoveSpeed * MoveDirection * DeltaTime, 0.0f, 0.0f));
    }
}
```

- 수정 코드

```c++
void AMovingPlatform::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    if (!FMath::IsNearlyZero(MoveSpeed))
    {
        if (FMath::Abs(StartLocation.X - GetActorLocation().X) > MaxRange) {
            MoveDirection *= -1;
        }
        AddActorLocalOffset(FVector(MoveSpeed * MoveDirection * DeltaTime, 0.0f, 0.0f));
    }
}
```

- FMath::Abs를 사용했다.
  - 시작지점의 X값과 현재 X값을 비교해서 절대값비교
- 발판 이동속도도 너무 느린것같아서 에디터에서 기존 50.0f에서 150.0f로 3배 늘려주었다.

![](https://velog.velcdn.com/images/kyu_/post/0726e92e-6cbc-415e-83a1-0055dd498f21/image.gif)
