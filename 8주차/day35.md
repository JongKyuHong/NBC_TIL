# 언리얼 C++강의

## 랜덤위치에 아이템 스폰하기

### 콜리전 컴포넌트로 스폰 영역 지정하기

- 언리얼 엔진에서는 직접 지정한 박스, 구체, 캡슐 등 원하는 형태의 '콜리전 컴포넌트' 내부에서 임의의 좌표를 뽑아 아이템(액터)을 스폰할 수 있다.

```c++
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SpawnVolume.generated.h"

class UBoxComponent;

UCLASS()
class SPARTAPROJECT_API ASpawnVolume : public AActor
{
    GENERATED_BODY()

public:
    ASpawnVolume();

		UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Spawning")
    USceneComponent* Scene;
    // 스폰 영역을 담당할 박스 컴포넌트
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Spawning")
    UBoxComponent* SpawningBox;

    // 스폰 볼륨 내부에서 무작위 좌표를 얻어오는 함수
    UFUNCTION(BlueprintCallable, Category="Spawning")
    FVector GetRandomPointInVolume() const;
    // 특정 아이템 클래스를 스폰하는 함수
    UFUNCTION(BlueprintCallable, Category="Spawning")
    void SpawnItem(TSubclassOf<AActor> ItemClass);
};
// SpawnVolume.h
```

- UBoxComponent\* SpawningBox;

  - 박스 형태의 콜리전 영역
  - 언리얼 엔진의 `UBoxComponent`는 박스 내부에서 오버랩이나 충돌을 감지할 수 있는 컴포넌트
  - 실제로 눈에는 안보이고, 박스형태의 충돌 범위만 제공

- GetRandomPointInVolume()
  - SpawningBox 범위 내부에서 랜덤좌표(FVector)를 리턴
- SpawnItem()
  - 파라미터로 받은 아이템 클래스를 SpawnVolume내부의 랜덤위치에 생성

## 캐릭터 체력쪽

### UGameplayStatics

- 언리얼에서 게임플레이 관련된 유틸리티 함수들을 모아놓은 정적 클래스
- 위치: Engine 모듈에 속해있으며, #include "Kismet/GameplayStatics.h"를 추가해야 사용 가능
- 주요 용도: 데미지 적용, 사운드 재생, 스폰, 월드 관련 작업 등 게임플레이 전반의 편리한 기능 제공.

# 코드카타

## 배열의 곱셈

```c++
#include <string>
#include <vector>

using namespace std;

vector<vector<int>> solution(vector<vector<int>> arr1, vector<vector<int>> arr2) {
    vector<vector<int>> answer(arr1.size(), vector<int>(arr2[0].size(), 0));

    for (int i = 0; i < arr1.size(); i++){
        for (int j = 0; j < arr2[0].size(); j++){
            for (int k = 0; k < arr1[0].size(); k++){
                answer[i][j] += arr1[i][k] * arr2[k][j];
            }
        }
    }

    return answer;
}
```

- 배열의 곱셈은 행렬 A가 `m * n`이고 행렬 B가 `n * r`크기일때 가능하다.
  - 앞 행렬의 열의 개수와 뒤 행렬의 행의 개수가 같아야 한다.
  - 결과행렬 C는 `(앞행렬의 행수) * (뒤 행렬의 열수) = m * r`이 된다.
