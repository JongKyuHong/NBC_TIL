# 게임 개발자를 위한 C++ 문법

## Unreal Engine 기본개념

### 특징

- 실시간 렌더링 기능
  - 실시간 렌더링으로 최상의 비주얼을 손쉽게 구현
- 블루프린트
  - 프로그래밍을 몰라도 게임 로직을 만들 수 있음
  - 눈으로 확인하면서 개발 가능
- 모든 플랫폼에서 가능
  - PC, 콘솔, 모바일까지 한 번에 지원가능
  - 한번의 구현으로 다양한 기기에 배포가 가능하니 효율이 좋다.
- 활성화된 커뮤니티
  - 튜토리얼이 잘 작성되어 있고 포럼이 활성화 되어 있다.

### 액터배치

C++클래스를 새로만들고 Visual Studio에서 BeginPlay()에 로그를 출력하는 문을 넣었다.

```c++
void AMyActor::BeginPlay()
{
	Super::BeginPlay();

	UE_LOG(LogTemp, Warning, TEXT("Hello, World"));
}
```

> 문제발생

코드를 정상적으로 적고 액터를 배치하고 언리얼 에디터에서 실행했음에도 로그가 뜨지 않는 문제가 발생했다.

> 해결

- 언리얼 에디터, visual studio끄기
- visual studio키고 솔루션 클리어 -> 솔루션 빌드 수행
- 언리얼 에디터 다시키고 play하니 정상적으로 로그 출력됨

### UE_LOG

- 언리얼 엔진의 출력 로그에서 확인
- 카테고리, 심각성, 출력할 내용으로 나뉜다.

### 출력 형식

- 로또 번호 출력할때

```c++
for (int i = 0; i < 6; i++)
{
	UE_LOG(LogTemp, Warning, TEXT("%d Num = %d"), i + 1, Numbers[i]);
}
```

이렇게 출력했는데 완성된 모습을 보니까

```
[출력 예시]
//[LogTemp]는 실제 출력하는게 아니고 카테고리입니다.
[LogTemp] Lotto Numbers: 3, 15, 22, 30, 35, 44
```

이렇게 한줄로 출력하는데 아이디어가 생각이 안났다.

```c++
FString LottoString = TEXT("Lotto Numbers:");

for (int i = 0; i < 6; i++)
{
	LottoString += FString::FromInt(Numbers[i]);
	if (i < 5)
	{
		LottoString += TEXT(",");
	}
}
UE_LOG(LogTemp, Warning, TEXT("%s"), *LottoString);
```

FString을 활용해서 해결했다.

# C++ 이것저것

## 멤버 초기화 리스트

```c++
class Circle {
private:
    double radius;
public:
    Circle(double r) : radius(r) {}
}
```

만약 이러한 코드가 있고, r이 0보다 큰지 검사하려면 어떻게 할까?
멤버 초기화 리스트가 생성자 본문에 적는것보다 성능이 좋다는것은 알고있지만 검사하는 로직이 필요하기 때문에 만약 나라면 본문에 r이 0보다 크다면이라는 조건문을 달아서 구현했을것같다.

### 헬퍼 함수 사용하기

멤버 초기화 리스트에 조건을 달아주기 위한 헬퍼함수를 만드는법을 공부했다.

```c++

static double ValidateRadius(double r){
  if (r <= 0){
    throw std::invalid_argument("Radius must be positive");
  }
  return r;
}

Circle(double r) : radius(ValidateRadius(r)) {}
```

이렇게 헬퍼함수를 사용하면 리팩토링할때도 유효하고, 멤버 초기화 리스트의 성능적인 이점도 챙길 수 있을것같다.

## 코드카타

- x만큼 간격이있는 n개의 숫자

```c++
vector<int> solution(long long n) {
    vector<int> answer;
    string tmp = to_string(n);
    for (int i = tmp.length()-1; i >= 0; i--){
        answer.push_back(tmp[i]-'0');
    }
    return answer;
}
```

- string형태인 tmp의 tmp[i]를 int형 벡터에 넣으려니까 아스키코드값이 출력되었다.
- string을 int에 넣을때 꼭 -'0'을 써주어서 0의 아스키코드값을 빼서 내가 원하는 숫자를 넣도록 하자

## 세션

### Vector의 resize와 reserve차이

- resize()
- 메모리 할당 + 모든 요소를 초기화
- size와 capacity 모두 변경
- vector.resize()

  - vector.size() = 100, vector.capacity() = 100

- reserve()
- capacity만 변경 size는 그대로
- 메모리만 할당 + 초기화는 안함
- vector.reserve(100)
  - vector.size() = 0, vector.capacity() = 100

### SOLID

- SRP
  - 클래스는 변경 이유가 하나여야 한다.
  - 인벤토리 클래스가 아이템 보관 뿐만아니라 UI출력까지 담당하면 UI 요구사항 변경이 인벤토리 로직까지 흔들게 된다.
  - 이때 인벤토리와 UI로직을 분리하면 SRP에 가까워진다.
- OCP
  - 기능 확장은 쉽고 기존 코드 수정은 최소화
  - switch문으로 무기를 추가하게 되면 무기가 추가될때마다 switch분기가 추가되어야 하는데 이 대신 인터페이스/가상함수로 확장하면 무기가 추가될때 기존 계산 코드는 그대로 둘 수 있다.
- LSP
  - 자식 타입은 부모타입 자리에 넣어도 프로그램의 의미가 깨지면 안된다.
  - 부모인 새 클래스에 fly가 있는데 자식으로 펭귄이 상속받아서 fly에서 예외를 던진다면 안전하지가 않다. 호출부에서 타입을 체크해야 하고 다형성이 망가짐
- ISP
  - 인터페이스 분리 , 클라이언트는 사용하지 않는 메서드에 의존해서는 안된다.
  - scv, 마린을 예시로 보았을때 마린에 자원 채취메서드가 있고, scv에 스팀팩 메서드가 있으면 의미가 없다. 필요한 기능을 쪼개어서 각 클래스는 필요한 인터페이스만 구현하게 되면 설계가 깔끔해진다.
- DIP
  - 고수준 정책과 저수준 구현이 모두 '추상'에 의존하게 하라, 상위 모듈이 하위 모듈에 직접 의존하면 안됨
  - DamageSystem이 FireAttackStrategy같은 구체 클래스를 직접 생성/의존하면 공격 방식 추가시 고수준 로직이 같이 바뀝니다. IAttackStrategy에 의존하고(주입받도록) 구현체만 교체하면, 시스템 로직은 변하지 않고 확장만 일어납니다.

## 총 정리

- Unreal Engine은 실시간 렌더링, 블루프린트, 멀티 플랫폼 지원, 활발한 커뮤니티를 강점으로 가진다.
- 액터 배치/실행 시 `BeginPlay()` 로그가 안 뜰 수 있으므로,
  - 에디터/IDE를 종료한 뒤
  - 솔루션 클리어 → 빌드
  - 에디터 재실행 후 Play
    같은 절차로 빌드 결과가 에디터에 정상 반영되었는지 점검한다.
- `UE_LOG(카테고리, 심각성, TEXT("포맷"), ...)` 형태로 출력 로그를 남기며,
  - 반복 출력은 `for`로 여러 줄 로그를 찍을 수 있고,
  - “한 줄로 보기 좋게 출력”하려면 `FString`을 누적해 `TEXT("%s")`로 출력하는 방식이 유용하다.
- C++ 멤버 초기화 리스트는 성능/명확성 측면에서 유리하지만, 유효성 검사가 필요하면
  - `ValidateRadius()` 같은 헬퍼 함수를 만들어 초기화 리스트에서 검증 로직을 수행하는 패턴을 사용할 수 있다.
- 문자열의 문자(`'0'` ~ `'9'`)를 숫자로 바꿀 때는 `tmp[i] - '0'`처럼 ASCII 오프셋을 빼서 원하는 정수 값을 얻는다.
- `vector::resize()` vs `vector::reserve()`
  - `resize(n)`: size 변경 + 요소 초기화까지 수행(보통 size/capacity가 함께 커짐).
  - `reserve(n)`: capacity만 확보(size는 그대로), 요소 초기화는 하지 않음.
  - “요소 개수 자체를 늘릴지”는 `resize`, “미리 공간만 확보할지”는 `reserve`를 선택한다.
- SOLID 적용 예시 요약
  - SRP: 인벤토리 로직과 UI 출력 책임을 분리해 변경 이유를 하나로 유지한다.
  - OCP: `switch` 분기 대신 인터페이스/가상함수로 무기(행동) 확장을 수용한다.
  - LSP: `Bird`에 `Fly()`가 있을 때 펭귄이 예외를 던지는 식이면 치환이 깨져 타입 체크가 필요해진다.
  - ISP: 마린/SCV처럼 역할이 다른데 한 인터페이스에 기능을 몰아넣지 말고 필요한 기능 단위로 쪼갠다.
  - DIP: `DamageSystem`이 구체 전략에 직접 의존하지 않고 `IAttackStrategy` 같은 추상에 의존하도록 주입 구조로 만든다.
