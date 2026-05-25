![](https://velog.velcdn.com/images/kyu_/post/0b751f87-5c8d-49d0-b523-f3e311493c34/image.png)

# Unreal C++ 강의 복습

## Spawn

- SpawnVolume이라는 Actor를 상속한 클래스를 하나 만듬
- UBoxComponent\* SpawningBox : 박스 형태의 콜리전을 가짐, 박스형태는 눈에는 보이지 않지만 오버랩, 충돌이 감지가능
- GetRandomPointInVolume : 스포닝박스 안에서 랜덤좌표를 리턴해줌
- SpawnItem : 파라미터로 받은 아이템클래스를 SpawnVolume 내부의 랜덤 위치에 생성

## Item Table만들기

- CSV, JSON 파일로 관리해서 엔진안으로 임포트하고, 코드나 블루프린트에서 쉽게 불러 쓸 수 있다.
  - 엑셀에서 숫자만 바꾸면 되어서 편함
- 우선 데이터 테이블의 행을 C++구조체로 매핑해야 한다. 언리얼 엔진은 FTableRowBase라는 기본 구조체를 제공한다.
  - 이를 상속한 구초제를 만들면 CSV, JSON 행을 구조체에서 정해준 형태로 받아올 수 있다.

## 캐릭터 체력 시스템 구현

- PlayerState
  - 언리얼에서 PlayerState는 주로 멀티플레이 환경에서 각 플레이어 간 데이터 동기화를 위해 사용
  - ex) 점수, 킬/데스 카운터처럼 서버와 클라이언트 모두가 공통으로 확인해야 하는 정보
  - 싱글에서는 캐릭터클래스 자체에 체력이나 스코어변수를 넣어서 관리해도 무방

### 지뢰아이템

- 지뢰 아이템이 폭발할때 주변 액터에게 데미지를 주려면 `UGameplayStatics::ApplyDamage` 함수를 호출해 해당 액터의 `TakeDamage()`가 실행되도록 하면 된다.
- `UGameplayStatics`는 언리얼 엔진의 `Engine` 모듈안에 정의된 클래스
  - 게임에서 자주쓰이는 기능(사운드 재생, 액터 스폰, 데미지 등) 월드 어디서든 편하게 쓸 수 있도록 모아놓은 헬퍼 클래스
  - 모든 AActor들은 TakeDamage라는 가상함수를 가지고 있음
    - 우리가 클래스에서 TakeDamage를 오버라이드 했다면 로직이 실행된다.

## 점수 관리 시스템

- GameMode
  - 게임의 규칙 정의/관리
  - 어떤 캐릭터를 스폰?, 플레이어 사망 시 어떻게 처리할지
  - 멀티플레이에서 서버 전용으로 동작
- GameState
  - 공유되어야 하는 전역상태 저장
  - 점수, 남은시간, 현재 게임 단계, 스폰된 오브젝트의 총 개수 등
  - 멀티플레이에서는 서버가 관리하고, 클라이언트는 이를 자동으로 동기화 받아볼 수 있다.

## GameState를 이용한 게임 루프 구현하기

- GameState는 게임 전반에 걸쳐 모든 플레이어가 공유해야하는 상태를 담는 클래스
- 게임흐름 로직관리
  - 레벨은 3개
  - 각 레벨당 30초
  - 코인 모두 획득 시 다음 레벨
  - 3레벨이 끝나면 Game Over
  - 레벨 시작시 40개의 아이템을 생성하고 그 중 코인 아이템이 몇개 생성되었는지 추적(SpawnCoinCount)하고, 플레이어가 먹은 코인 개수를 추적(CollectedCoinCount)하여 모두먹으면 즉시 레벨 종료
  - 레벨 종료되면 마지막 레벨인지 레벨인덱스로 확인하고 아니라면 UGameplayStatics::OpenLevel로 다음레벨

- OpenLevel을 호출하면 현재 월드가 언로드되고, 새로운 맵이 로드되어서 다시 BeginPlay가 호출된다. 이때 GameState도 새로 생성되므로 기존에 유지되던 변수가 모두 초기화 될 수 있다.

## GameInstance를 활용한 데이터 유지

- 레벨 전환시에는 GameMode, GameState같은 기본 클래스, 맵 내에서 생성된 대부분의 객체가 처음부터 다시 생성된다.
- 이전 정보를 기억하고 싶을때 두가지 방법이 있다.
- Game Instance
  - 프로젝트가 시작될 때부터 완전히 종료될때까지 살아있는 객체
  - 맵이 전환되어도 파괴되지 않음, 여기서 전역 데이터 유지 가능

- Seamless Travel
  - 멀티플레이 환경에서 주로 사용하는 레벨 전환 방식, GameState/PlayerController등을 파괴하지 않고 그대로 다음 맵으로 넘어가는 기능
  - 대부분의 객체를 유지할 수 있지만 설정과 로직이 조금 더 복잡

## 게임 루프 요약

- 게임 실행
  - GameInstance생성, GameMode/GameState 생성, 첫 레벨 로드

- BeginPlay()
  - AMyGameState::BeginPlay() -> StartLevel()
  - 스폰 볼륨에서 40개 아이템 스폰
  - 코인 개수 추적
  - 30초 타이머 시작
- 플레이어가 코인 획득
  - CoinItem::ActivateItem에서 GameState->AddScore(), OnCoinCollected()
  - 모든 코인을 모으면 즉시 EndLevel()

- 레벨 종료
  - EndLevel()에서 CurrentLevelIndex++
  - 남은 레벨이 있으면 UGameplayStatics::OpenLevel로 다음맵 로드
  - 더이상 레벨이 없으면 GameOver()

- 다음 맵 로드 시
  - 새로운 GameState생성 -> 다시 BeginPlay()->StartLevel()
  - 이전 레벨에서 유지하고 싶은 정보는 GameInstance에서 관리
- GameOver
  - UI표시

# 트러블 슈팅

## Player 태그

- 아이템과 캐릭터가 충돌해도 사라지지도, 로그가 출력되지도 않았다.
- 구석구석 찾아보니 플레이어에 Tag를 안달아주었다.
- Tag를 달아주니 정상적으로 출력, 파괴되었다.

## 스폰된 코인 개수 오류

![](https://velog.velcdn.com/images/kyu_/post/5bb70487-88cc-40fe-9521-90ae89072b70/image.png)

- 총 스폰된 코인의 개수가 0으로 나옴

```c++
AActor* ASpawnVolume::SpawnRandomItem()
{
    if (FItemSpawnRow* SelectedRow = GetRandomItem())
    {
        if (UClass* ActualClass = SelectedRow->ItemClass.Get())
        {
            SpawnItem(ActualClass);
        }
    }
    return nullptr;
}
```

- 타고 올라가니 SpawnVolume에서 return이 무조건 nullptr로 되어있었음
- return SpawnItem(ActualClass)로 변경하여 해결

![](https://velog.velcdn.com/images/kyu_/post/70694cf4-da95-450f-be9a-5e58b6904df3/image.png)

## HUD와 Score 연동

![](https://velog.velcdn.com/images/kyu_/post/5b21324e-1ef6-4745-be10-4d1f31bc01ae/image.png)

- 코인을 먹어도 먹은 개수는 오르나 HUD의 Score는 오르지 않음

```c++
void AMyGameState::AddScore(int32 Amount)
{
    if (UGameInstance* GameInstance = GetGameInstance())
    {
        UMyGameInstance* MyGameInstance = Cast<UMyGameInstance>(GameInstance);
        if (MyGameInstance)
        {
            MyGameInstance->AddToScore(Amount);
        }
    }
}
```

- GameState의 AddScore에서 GameInstance의 TotalScore를 변경해주어서 해결
