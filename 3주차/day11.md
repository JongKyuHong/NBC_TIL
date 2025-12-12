# C++

## 도전실습

### 미니 실습

- [내 풀이] (https://github.com/JongKyuHong/NBC_TIL/blob/main/3%EC%A3%BC%EC%B0%A8/code/day11/Q3.cpp)
- this->Speed는 Speed라는 이름이 중복되었을때 말고는 굳이 쓸필요없다.
- 생성자에서 string& Name으로 매개변수를 받을때 문제가 되었었다. const로 바꾸니까 해결

### 도전 실습

- [내 풀이] (https://github.com/JongKyuHong/NBC_TIL/blob/main/3%EC%A3%BC%EC%B0%A8/code/day11/Q4.cpp)

![](https://velog.velcdn.com/images/kyu_/post/4d7c8c60-9061-48f5-8cbf-98db6b12bc49/image.png)

- 멤버함수만들때 const를 붙이라는 알림이 나옴
- 멤버함수에 const를 붙이는 기준은 객체(this)의 상태를 변경하지 않으면 붙이면 된다.
- getter함수이기 때문에 단순히 반환만 해주어서 const를 붙이게 되었다.

#### cppLint특강

- 특강 듣고 도전실습 코드 돌렸더니 에러가 136개가 나왔다ㄷㄷ

- [개선풀이] (https://github.com/JongKyuHong/NBC_TIL/blob/main/3%EC%A3%BC%EC%B0%A8/code/day11/Q4_1.cpp)
- 개선!

### 도전X도전 실습

![](https://velog.velcdn.com/images/kyu_/post/a9dbbf91-bcc9-4f6d-ba5b-413498543f02/image.png)

- 멤버함수에 const를 쓸때는 호출하는 다른 멤버함수도 const화 되어있어야 한다.

![](https://velog.velcdn.com/images/kyu_/post/37a82408-461b-486d-8bd7-a434400b14a2/image.png)

- 멤버초기화 리스트에서 배열 처음 사용해봄 {}

![](https://velog.velcdn.com/images/kyu_/post/a66a7b77-2eaa-4d8e-9369-64f0ab033e65/image.png)

- Aircraft클래스에서 Pilot crew[2]로 정의되어있는데 만약에 입력이 덜들어 올것을 대비해서 Pilot에 기본 생성자 만들어줌

- [내 풀이] (https://github.com/JongKyuHong/NBC_TIL/blob/main/3%EC%A3%BC%EC%B0%A8/code/day11/Q5.cpp)

### 4. 참고자료 개념

#### 1. #pragma once

```c++
#ifndef STUDENT_H_
#define STUDENT_H_

// 헤더 파일 내용
class Student {
    // ...
};

#endif
```

Include Guards방식에서는 #ifndef, #endif를 활용해서 정의되어있는 헤더를 다시 컴파일하지 않게 막았음

> 장점

- C/C++ 표준이라 모든 컴파일러에서 작동
- 명시적이다.

> 단점

- 코드가 길고 번거로움
- 매번 헤더파일을 읽어야함

그래서 등장한게 `#pragma once`이다.

```c++
#pragma once

// 헤더파일 내용
...
```

- 컴파일러가 파일의 물리적 위치를 기억함
- 같은 파일을 다시 include하면 아예 파일을 열지도 않고 건너뜀
- 파일 시스템 레벨에서 중복 체크

> 장점

- 한줄로 간결
- 매크로 이름 충돌 걱정 없음
- 컴파일 속도 더 빠름(파일 안읽으므로)
- Visual Studio가 자동생성 (기본 값)

> 단점

- 비표준
- 파일 내용이 동일하면 다른 파일도 같은 것으로 인식할 수 있음

#### 2. 캡슐화

- 객체 지향 프로그래밍의 핵심 원리
- 데이터(멤버 변수)와 메서드(멤버 함수)를 하나로 묶고 외부에서 직접 접근을 제한

> 특징

1. 번들링

- 관련된 데이터와 기능을 하나의 단위 (클래스)로 묶음

2. 은닉

- 내부 구현 세부사항을 외부에 숨김
- 공개 인터페이스만 제공

> 장점

- 사용하기 쉽다
- 프로그램 복잡성 감소
- 실수 방지
- `유지보수 용이`
- `코드 재사용성 향상`

#### 3. 선언(Declaration) vs 정의(Defintion)

- 선언은 컴파일러에게 알리는것
  - 타입과 이름만 알림
  - 메모리 할당 없음
  - 여러 번 가능
- 정의는 실제로 만듬
  - 메모리 할당됨
  - 단 한번 가능

> 궁금한거

```
int a = 10;
a = 20;
```

##### 이런경우는 뭐냐??

-> 대입과 정의를 비교해야 한다.
메모리 입장에서는

```c++
int a = 10; // 정의 메모리주소 0x1000에 공간생성 10저장

a = 20; // 같은 메모리주소 0x1000의 값만 변경

// 정의가 한번 만 가능하다는 것은
int a = 30; // 이렇게 a를 또 쓰면 에러
```

#### 4. 소멸자

객체가 소멸될때 자동으로 호출되는 특별한 멤버 함수
`~` 붙임
정리작업 수행
생성자 반대 역할
매개변수, 반환값 없음(void도 안붙임)
public선언

#### 5. this포인터

객체 자신의 주소를 가리키는 포인터
모든 비정적 멤버 함수에 자동으로 전달
타입 : 클래스명\* const (포인터 상수)

> 특징

- 포인터 상수 : 값 변경 불가

#### 6. const 멤버 함수

객체 멤버 변수를 변경하지 않겠다고 보장하는 함수

> 특징

1. 멤버 변수 변경 불가
2. non-const 멤버 함수 호출 불가 (오늘 아까 구현하다 알게된거)
3. const 객체는 const 함수만 불러올 수 있다.
   non-const

```c++
class Student {
 public:
  void study() {  // non-const
    cout << "공부 중\n";
  }

  void showInfo() const {  // const
    cout << "정보 출력\n";
  }
};

const Student s;
s.study();     // ❌ 에러! const 객체는 non-const 함수 호출 불가
s.showInfo();  // ✅ OK! const 함수는 호출 가능
```

> 그래서 왜씀?

1. 의도 명확화
2. 실수 방지
3. const 객체 지원
4. 컴파일러 최적화

- const는 값을 안바꾸니 최적화 가능

### 이전 학습가이드 참고자료

#### 1. 포인터, 레퍼런스

- 포인터는 메모리를 저장하는 변수

  - `*` : 포인터 선언 및 역참조
  - `&` : 주소 가져오기
  - 포인터 자체가 변수이므로 4~8바이트 메모리 차지 (CPU레지스터에 따라 다름)
  - NULL값 가능 (레퍼런스와 비교했을때의 장점)
  - 재할당 가능 (다른 주소 가리키기 가능)

- 레퍼런스는 기존 변수의 별명(alias)
  - 별도 메모리 공간 없음(컴파일러가 최적화)
  - NULL불가
  - 선언시 바로 초기화해야됨
  - 재할당 불가 (다른 변수 가리킬 수 없음)

#### 2. const와 포인터

> const int\* p1

```c++
int a = 10;
const int* p1 = &a;

*p1 = 20;  // ❌ 에러! 가리키는 값 변경 불가
p1 = &b;   // ✅ OK! 포인터 자체는 변경 가능

```

- 가리키는 값을 const로 취급하겠다
- 포인터가 가리키는 데이터를 읽기 전용으로 만듦
- 포인터 자체는 다른 주소를 가리킬 수 있음

> int\* const p2

```c++
int a = 10, b = 20;
int* const p2 = &a;

*p2 = 30;  // ✅ OK! 가리키는 값 변경 가능
p2 = &b;   // ❌ 에러! 포인터 자체 변경 불가
```

- 포인터 자체가 const
- 포인터가 가리키는 주소를 바꿀 수 없음
- 포인터가 가리키는 값은 변경 가능

> const int\* const p3

```c++
int a = 10, b = 20;
const int* const p3 = &a;

*p3 = 30; // ❌ 에러! 가리키는 값 변경 불가
p3 = &b; // ❌ 에러! 포인터 자체 변경 불가
```

- 포인터도 const, 가리키는 값도 const
- 가장 엄격한 제약
- 읽기 전용 데이터를 가리키는 고정 포인터

#### 3. 더블 포인터

- 포인터의 주소를 저장하는 포인터

```c++
int a = 10;
int* p = &a;
int** pp = &p;

cout << a; // 10
cout << *p; // 10
cout << **pp; // 10
```

> 사용사례

##### 1. 함수에서 포인터 자체를 변경

포인터는 값(주소)을 저장하는 변수이므로, 함수에서 포인터를 변경하려면 포인터의 포인터가 필요하다.

- 더블 포인터가 있다면

```c++
void allocateMemory(int** pp){
  *pp = new int(100); // 포인터가 가리키는 곳 변경
}

int main(){
  int* ptr = nullptr;
  allocateMemory(&ptr); // ptr자체를 변경
  cout << *ptr; // 100
  delete ptr;
}
```

- 더블포인터가 없다면

```c++
void allocateMemory(int* p){
  p = new int(100);
}
int main(){
  int* ptr = nullptr;
  allocateMemory(ptr); // ptr은 여전히 nullptr(복사본만 변경됨)
}
```

##### 2. 2차원 배열(동적 할당)

```c++
int** matrix = new int*[rows]; // 행 배열
for (int i = 0; i < rows; i++){
  matrix[i] = new int[cols];
}

matrix[0][0] = 10;

for (int i = 0; i < rows; i++){
  delete[] matrix[i];
}

delete[] matrix;
```

##### 3. 함수에서 배열의 배열 전달

```c++
void printMatrix(int** matrix, int rows, int cols) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            cout << matrix[i][j] << " ";
        }
        cout << "\n";
    }
}

```

- 벡터는 그냥 넘기면 됨

##### 4. Linked List 노드 삭제

```c++
struct Node {
    int data;
    Node* next;
};

void deleteNode(Node** head, int value) {
    if (*head == nullptr) return;

    if ((*head)->data == value) {
        Node* temp = *head;
        *head = (*head)->next;  // head 포인터 자체를 변경!
        delete temp;
        return;
    }
}
```

## 알고리즘 문제 풀어보기

### 1.

- [FizzBuzz] (https://www.acmicpc.net/problem/28702)

- 문자열을 숫자로 바꾸는것에 대해서 많이 생각하게 되는 문제
- stoi의 경우 숫자가 아니면 error를 발생해서 try/catch문으로 받아야하는데 이거는 알고리즘에서는 크게 의미 없는거 같고
- isdigit은 0~9까지의 일의자리 숫자만 가능하다.
- 그래서 현실적으로는 문자열을 하나씩 뜯어봐서 isdigit을 각각 쓰는게 나아보인다.

- [내 풀이] (https://github.com/JongKyuHong/NBC_TIL/blob/main/3%EC%A3%BC%EC%B0%A8/code/day11/Q1.cpp)

### 2.

- 이 문제는 정렬 메서드 사용법을 익히는데 많이 도움이 되었다.
- 외부함수 사용, 내부에서 사용
- 정렬 조건
- 중복제거를 위해서 set을사용 or 벡터에서 중복제거
- 벡터에서 중복제거를 한다면 입력에서 걸러내기 or 일단 다 입력받고 erase와 unique로 중복 지우기

#### 외부함수 사용

```c++
sort(vec.begin(), vec.end(), [](const string& a, const string& b)
	{
		if (a.length() != b.length())
		{
			return a.length() < b.length();
		}
		return a < b;
	});
```

기존에는 이렇게 사용했다.

```c++
bool compare(string& a, string& b)
{
	if (a.length() != b.length())
	{
		return a.length() < b.length();
	}
	return a < b;
}

sort(vec.begin(), vec.end(), compare);
```

- sort의 세번째인자는 함수포인터(함수 자체)를 요구하기 때문에 인자를 안넘겨줘도 된다.
- vec의 길이가 1이라면 자동으로 sort는 생략된다 그래서 vec.size() > 1 분기를 안넣어줘도 됐음

#### vector로 중복제거 - unique사용

```c++
vec.erase(unique(vec.begin(), vec.end()), vec.end());
sort(vec.begin(), vec.end(), compare);
```

- 기존에는 erase하고 sort하는 방식이였는데
- unique가 바로 옆에 붙은 중복을 제거하고 떨어진 중복은 제거하지 못한다고 한다..

```c++
sort(vec.begin(), vec.end(), compare);
vec.erase(unique(vec.begin(), vec.end()), vec.end());
```

- sort하고 unique + erase로 변경
- 그래도 이 방식이 입력받을때 find(vec.begin(), vec.end(), inp) == vec.end() 이 조건으로 계속 체크하는것보다는 효율적이다.

- [내 풀이] (https://github.com/JongKyuHong/NBC_TIL/blob/main/3%EC%A3%BC%EC%B0%A8/code/day11/Q2.cpp)

## 궁금한거

### CPU의 아키텍처, OS비트수 의미

- CPU 아키텍처는 CPU가 한번에 처리할 수 있는 데이터 크기
- 내부 레지스터 크기

- OS비트수는 운영체제의 메모리 관리
- 프로그램에게 제공하는 주소공간 크기

- CPU가 더크면 OS에서 돌아가지만 OS의 비트수가 더 크면 못돌림
  - 64비트OS에서 32비트 아키텍처 CPU사용불가

### int\* const p2는 어디서 사용하는가??

- 파일 포인터 같은곳에 사용한다.
- 파일을 가리키는 포인터 자체는 변하면 안되지만 파일 내용은 변해야 하므로

### explicit이란??

> 생성자나 변환 연산자 앞에 붙여서 컴파일러의 암묵적(묵시적)형변환을 방지하는 키워드

explicit = 명시적인

> 장점

- 예상치 못한 암묵 변환 방지
- 타입 안정성, 코드 안정성 강화
- 읽기 쉬운 코드 / 유지보수성 향상
- 오버로드 함수 해석에서 모호성 최소화

> 단점

- 코드가 다소 장황해짐
- 의도적인 암묵 변환 막을 수 있음
- 불필요한 오버헤드

> 사용할때

- 자동변환이 위험한경우 항상 explicit사용 권장
- 암묵 변환 막고 싶을때
