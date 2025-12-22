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

## 기타 등등

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
