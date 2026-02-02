![](https://velog.velcdn.com/images/kyu_/post/31890060-e76b-439f-9ecc-7417cd1c564b/image.png)


# 과제

## 구현

### 6. 웨이브별로 변주주기

- 우선 웨이브별로 다른 아이템이 등장하게 웨이브 아이템 에셋을 만들었다.

#### 독, 힐, 슬로우 포션

- 각각에 해당하는 GE를 만들고 2웨이브 -> 슬로우 포션, 3웨이브 -> 독포션을 추가하였다.

### 7. 증강별 이미지, 색상 넣기

![](https://velog.velcdn.com/images/kyu_/post/0444aa5e-ac8c-4563-8178-64b793eaee03/image.png)

- Set Backgound Color로 증강 단계에 따라 색을 넣고
- Set Brush from Texture노드로 각 증강에 맞는 이미지를 띄워주었다.

![](https://velog.velcdn.com/images/kyu_/post/b57faa84-c7e9-466f-8d49-de7fe8232120/image.png)
- 실버 증강


![](https://velog.velcdn.com/images/kyu_/post/56e430e6-2c26-4d15-a792-8b6052588ccf/image.png)
- 골드 증강


![](https://velog.velcdn.com/images/kyu_/post/6d2d3c70-49b5-4145-99b6-bda0b2d6a7ab/image.png)
- 프리즘 증강
  - 프리즘 특유의 색상을 넣으려면 머티리얼을 만져야할거같아서 우선 보라색 표기

### 8. 나머지 마무리

- 종료 조건 개선
- 웨이브 마다 아이템 배치
- 디버프 관리
- 아이템 등장 표시, 웨이브마다 표시
- UI애니메이션 추가
- 기타 등등

## 트러블 슈팅

### 7. 맵 레벨 넘어갈때 스텟 적용하기

- 맵레벨이 넘어갈때 캐릭터 스케일이 제대로 적용되지 않았다.
- 또, 슬로우 포션을 먹은상태로 맵 레벨이 넘어가면 기본이동속도가 슬로우 포션을 먹은것처럼 초기화되는 버그가 있었다.

```c++
void AMyGameState::EndLevel()
{
  const UMyAttributeSet* AttributeSet = Cast<UMyAttributeSet>(ASC->GetAttributeSet(UMyAttributeSet::StaticClass()));
  if (AttributeSet)
  {
      MyGameInstance->Health = AttributeSet->GetHealth();
      MyGameInstance->MaxHealth = AttributeSet->GetMaxHealth();
      //MyGameInstance->MoveSpeed = AttributeSet->GetMoveSpeed();
      MyGameInstance->MoveSpeed = ASC->GetNumericAttributeBase(UMyAttributeSet::GetMoveSpeedAttribute());
      MyGameInstance->JumpZVelocity = AttributeSet->GetJumpZVelocity();
      MyGameInstance->CharacterScale = AttributeSet->GetCharacterScale();
      MyGameInstance->ExpGainRate = AttributeSet->GetExpGainRate();
      MyGameInstance->Level = AttributeSet->GetLevel();
      MyGameInstance->Exp = AttributeSet->GetExp();
      MyGameInstance->MaxExp = AttributeSet->GetMaxExp();
  }
}

void AMyGameState::StartLevel()
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

        float ScaleValue = MyGameInstance->CharacterScale;
        if (ScaleValue > 0.0f)
        {
            PlayerPawn->SetActorScale3D(FVector(ScaleValue));
        }
    }

}
```

- EndLevel에서 GameInstance에 스텟값을 저장한다.
  - MyGameInstance->MoveSpeed = ASC->GetNumericAttributeBase(UMyAttributeSet::GetMoveSpeedAttribute());
  - GetNumericAttributeBase를 통해 디버프 걸리기 전 순수한 스텟을 저장한다.

- StartLevel에서 스텟값을 가져올때
  - 캐릭터 스케일값을 PlayerPawn->SetActorScale3D(FVector(ScaleValue)); 로 지정해주었다.


# 코드카타

## XOR연산

- [테이블 해시 함수](https://school.programmers.co.kr/learn/courses/30/lessons/147354)

```c++
#include <string>
#include <vector>
#include <algorithm>
#include <iostream>

using namespace std;

int solution(vector<vector<int>> data, int col, int row_begin, int row_end) {
    int answer = -1;

    sort(data.begin(), data.end(), [col](auto& a, auto& b){
        if (a[col-1] != b[col-1]) return a[col-1] < b[col-1];
        return a[0] > b[0];
    });

    for (int i = row_begin-1; i <= row_end-1; i++){
        vector<int> target = data[i];
        int sumV = 0;
        for (int t : target){
            sumV += t % (i+1);
        }

        if (answer == -1){
            answer = sumV;
        } else {
            answer ^= sumV;
        }

    }
    return answer;
}
```

- XOR연산은 ^로 간단하게 할 수 있다는 것을 배웠다.

## 중복계산 ㄴ

- [시소짝꿍](https://school.programmers.co.kr/learn/courses/30/lessons/152996)

```c++
#include <string>
#include <vector>
#include <algorithm>
#include <unordered_map>
#include <iostream>
using namespace std;

long long solution(vector<int> weights) {
    long long answer = 0;
    unordered_map<int, int> um;
    sort(weights.begin(), weights.end());

    for (int i = 0; i < weights.size(); i++){
        int target = weights[i];
        if (um.count(target)) answer += um[target];
        if ((target*2)%3 == 0 && um.count(target*2/3)) answer += um[target*2/3];
        if (target%2 == 0 && um.count(target*1/2)) answer += um[target*1/2];
        if ((target*3)%4 == 0 && um.count(target*3/4)) answer += um[target*3/4];
        um[target]++;
    }

    return answer;
}
```

- 완탐으로 돌리기에는 입력이 너무 크다.
- 아이디어는 맞는데 계속 틀려서 애먹었던 문제
- 오름차순 정렬을 하고, unordered_map에 넣어주면서 계산을해서 중복 계산을 방지했다.
- um[실수값]이 되버리면 소수점 부분을 버리고 계산이 이상해지므로 나머지 계산으로 0인경우만 계산을 했다.
