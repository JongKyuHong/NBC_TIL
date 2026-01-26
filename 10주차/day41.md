# 챌린지 반

## GAS란??

- Gameplay Ability System
  - Epic Games에서 만든 공식 게임플레이 프레임워크
  - 수치관리를 직접 구현하면 생기는 한계점을 해결해 줌

## GAS의 5대 요소

- Ability : 캐릭터의 능력 (공격, 점프, 스킬)
- Attribute : 캐릭터의 수치(Health, Stamina, Mana)
- Effect : 변화(데미지, 힐, 버프, 디버프)
- Tags : 상태(Jumping, Stunned, Invincible)
- Cue : 연출(VFX, SFX)

### 직접 구현 vs GAS 비교

|       항목        |         직접 구현          |      GAS       |
| :---------------: | :------------------------: | :------------: |
|   네트워크 복제   | 직접 UPROPERTY(Replicated) |      자동      |
| Clamp (최소/최대) |     매번 FMath::Clamp      |      자동      |
|     변경 감지     |       직접 함수 호출       |   콜백 자동    |
|    버프/디버프    |     타이머로 직접 관리     | Effect로 자동  |
|    데미지 계산    |        복잡한 if문         | Execution Calc |
|    UI 업데이트    |         직접 Bind          | Delegate 자동  |
|      확장성       |            낮음            |      높음      |

- 자동 네트워크 복제가 가장큰 장점
- 멀티플레이 게임을 만들때 GAS는 필수이다.

## Attribute System

- Attribute란?
  - 캐릭터의 수치 데이터
    - Health
    - MaxHealth
    - Stamina
    - Mana
    - AttackPower
    - Defense등

- AttributeSet이란?
  - Attribute들의 집합

```c++
class UMyAttributeSet : public UAttributeSet {
    UPROPERTY(Replicated)
    FGameplayAttributeData Health;

    UPROPERTY(Replicated)
    FGameplayAttributeData MaxHealth;
};
```

- AttributeSet은 C++로 작성해야함
  - 네트워크 복제(UPROPERTY(Replicated))
  - Attribute매크로(ATTRIBUTE_ACCESSORS)
  - 콜백 함수(PreAttributeChange, PostGameplayEffectExecute)
  - 이런기능들이 C++에서만 가능하다.

### ASC (Ability System Component)

- 어빌리티 시스템 컴포넌트(ASC)는 게임플레이 어빌리티 시스템(GAS)전체를 관리하는 핵심 컴포넌트
- 게임플레이 어빌리티와 관련된 연산을 처리하는 중앙 처리 장치(CPU)와 같은 역할을 수행함
- 하나의 액터에는 하나의 ASC만 부착 가능
- 액터에 ASC가 부착되어 있어야 게임플레이 어빌리트를 활성화 할 수 있음
- ASC가 부착된 액터 사이에서 GAS를 이용한 상호작용이 가능

```c++
// ASC초기화
ASC->InitAbilityActorInfo(this, this);
```

```
Character (Owner)
    ↓
Ability System Component (ASC)
    ├── AttributeSet (Health, Stamina)
    │     ↓ (변경)
    ├── Gameplay Effects (Damage, Buff)
    │     ↓ (조건)
    ├── Gameplay Tags (State, Window)
    │     ↓ (발동)
    └── Gameplay Abilities (Attack, Jump)
          ↓ (연출)
      Gameplay Cues (VFX, SFX)
```

- 흐름
  - Effect가 Attribute변경
  - Tag 조건 체크
  - Ability 발동
  - Cue로 연출

## 실습

### 매크로

```c++
#define ATTRIBUTE_ACCESSORS(ClassName,PropertyName)\
GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName,PropertyName)\
GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName)\
GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName)\
GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)
```

- 매크로 가져오기

### GetLifetimeReplicatedProps

```c++
// AttributeSet 필수 함수
    virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
```

- AttributeSet에서 관리하는 속성(체력, 마나 등)이 멀티플레이어 환경에서 동기화되어야 한다면 필수로 사용해야 한다.
- 복제시스템에 이 변수들은 네트워크를 통해 공유할 목록이라고 등록하는 절차

### ReplicatedUsing, 매크로 호출

```c++
UPROPERTY(BlueprintReadOnly, Category = "Attributes", ReplicatedUsing = OnRep_Health)
FGameplayAttributeData Health;
ATTRIBUTE_ACCESSORS(UMyAttributeSet, Health)

UFUNCTION()
virtual void OnRep_Health(const FGameplayAttributeData& OldHealth);
```

- ReplicatedUsing = OnRep_Health
  - 아래 변수 Health가 서버에서 값이 바뀌면 클라이언트들에게 복제하라는 뜻
  - 클라이언트가 새로운 값을 받았을 때 OnRep_Health라는 함수를 자동으로 실행하라는 지시

- ATTRIBUTE_ACCESSORS(UMyAttributeSet, Health)
  - 코드 상단에 정의해두었던 매크로를 호출하는 것
  - GetHealth(), GetHealthAttribute(), SetHealth(float newVal), InitHealth(float NewVal)가 자동생성
  - 왜 매크로를 쓸까?
    - 각 속성마다 Getter/Setter를 만들기 어렵기 때문에 언리얼 GAS에서 공식적으로 사용하는 표준 관행

### PreAttributeChange, PostGameplayEffectExecute

```c++
// Attribute 변경 전 호출 (Clamping)
virtual void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) override;

// Attribute 변경 후 호출
virtual void PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data) override;
```

- PreAttributeChange는 수치가 바뀌기 직전 값의 범위를 교정
  - Clamp역할
  - 데미지 계산같은 복잡한 로직을 넣으면 안되고, Min, Max제한만 해야한다.

- PostGameplayEffectExecute
  - GameplayEffect에 의해 값이 변경된 후 실제 Event처리
  - 단순 수치변경 뿐아니라, 누가 누구에게 어떤 효과를 줬는지에 대한 모든 정보를 가지고 있다.
  - 주요 역할
    - 데미지 처리 : 체력이 0이 되었으니 캐릭터 사망 처리를 해라
    - UI 알림 : 방금 데미지를 입었으니 화면에 숫자를 띄워라
    - 클래핑 재확인 : 여기서 한번 더 범위를 잡아주는것이 안전

### 의존성 등록

```c++
PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "EnhancedInput", "GameplayAbilities" });

PrivateDependencyModuleNames.AddRange(new string[] { "GameplayTags", "GameplayTasks" });
```

# Unreal C++ 강의 복습

## 충돌 감지(Overlap)

```c++
// Overlap 이벤트 바인딩
Collision->OnComponentBeginOverlap.AddDynamic(this, &ABaseItem::OnItemOverlap);
Collision->OnComponentEndOverlap.AddDynamic(this, &ABaseItem::OnItemEndOverlap);

void ABaseItem::OnItemOverlap(
			UPrimitiveComponent* OverlappedComp,
			AActor* OtherActor,
			UPrimitiveComponent* OtherComp,
			int32 OtherBodyIndex,
			bool bFromSweep,
			const FHitResult& SweepResult)
{
	  // OtherActor가 플레이어인지 확인 ("Player" 태그 활용)
    if (OtherActor && OtherActor->ActorHasTag("Player"))
    {
				 GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Green, FString::Printf(TEXT("Overlap!!!")));
		    // 아이템 사용 (획득) 로직 호출
        ActivateItem(OtherActor);
    }
}

void ABaseItem::OnItemEndOverlap(
			UPrimitiveComponent* OverlappedComp,
			AActor* OtherActor,
			UPrimitiveComponent* OtherComp,
			int32 OtherBodyIndex)
{
}
```

- 충돌체(Collision)에 누군가 겹치기 시작하면(OnComponentBeginOverlap) -> 내가 만든 OnItemOverlap 함수를 실행하라
- OnComponentEndOverlap : 겹침 종료 시 실행

-

## 인터페이스

### 상속과의 차이점

- 상속은 부모클래스의 모든 속성과 기능을 자식 클래스가 물려받는 구조
  - 부모에서 구현한 로직을 자식에서 그대로 쓸 수 있고, 자식에서 재정의(오버라이딩)도 가능하다.

- 인터페이스는 설계도만 빌려주는 것
  - 반드시 만들어야 함
  - 실제 함수가 어떻게 동작할지는 각 자식에서 자유롭게 작성
  - 언리얼에서는 UINTERFACE 매크로를 사용하는 특수 클래스
    - 순수 가상함수만 들어있는 클래스와 다르다.

# 코드카타

## 큰수 만들기

```c++
#include <string>
#include <vector>
#include <stack>

using namespace std;

string solution(string number, int k) {
    string answer = "";

    stack<char> st;
    int index = 0;
    while (index < number.length()){
        while (!st.empty() && st.top() < number[index]){
            st.pop();
            k--;
            if (k == 0){
                break;
            }
        }
        if (k == 0){
            break;
        }

        st.push(number[index]);
        index++;
    }

    // 테스트케이스가 "10", 1이 들어왔을때 "10"이 출력됨 k가 남았을때 처리를 안함
    // 스택 위쪽이 작은값이므로 k개수만큼 빼줌
    while (k > 0 && !st.empty()) {
        st.pop();
        k--;
    }

    while(!st.empty()){
        answer = st.top() + answer;
        st.pop();
    }

    for (int i = index; i < number.length(); i++){
        answer += number[i];
    }

    return answer;
}
```

- [링크](https://school.programmers.co.kr/learn/courses/30/lessons/42883)
- 문자열에서 k개의 문자를 제거해서 가장 큰 숫자문자열을 만드는 문제
- 스택을 이용해서 쉽게풀었는데 테스트케이스 12번만 틀렸다.
  - 만약에 문자열이 이미 내림차순이 되어있는경우에 k개를 빼주지않았어서 while문을 하나 더 추가해서 k가 0이될때까지 스택에서 제거해주는 로직을 넣고 통과되었다.

## 점 찍기

```c++
#include <string>
#include <vector>
#include <cmath>
#include <iostream>

using namespace std;

long long solution(int k, int d) {
    long long answer = 0;

    for (int i = 0; i <= d; i += k){
        long long tmp = sqrt(pow(d,2) - pow(i ,2));
        answer += tmp / k;
        answer++;
    }

    return answer;
}
```

- [링크](https://school.programmers.co.kr/learn/courses/30/lessons/140107)
- d의 길이까지 몇개의 좌표가 있는가
- 행이나 열을 하나 고정하고 그것을 줄여가며 몇개의 점이 있는지 확인
- 매 순회마다 answer를 더해서 0인경우도 세주어야함
