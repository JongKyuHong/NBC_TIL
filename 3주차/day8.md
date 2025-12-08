# C++

## Class 개념

우리는 유지보수를 용이하게 하고, 재사용성을 높이는 방향으로 구현해야 합니다. C++에서는 이를 위해 객체지향 프로그래밍을 활용할 수 있습니다.

### class의 정의

- 자동차를 예로들면 우리는 자동차의 세부사항을 몰라도 기능을 이해하기 때문에 제어할 수 있습니다.
- 자동차에서 제공하는 기능 이외에 세부사항은 제어하지 못해야합니다. 왜냐면

  - 사용성이 떨어진다 : 사용자가 알아야할 정보가 많아지기 때문에
  - 구현 난이도가 증가한다 : 발생할 예외를 모두 고려해야 하므로

- 멤버 함수는 외부에서도 정의가능한듯
- class키워드를 사용할 때 기본값은 private

### 생성자

- 정의된 class를 변수로 선언하면 해당 객체가 메모리에 올라간다. 이를 인스턴스화 라고 함
- 생성자는 선언과 함께 정의도 해야한다.

### 헤더파일 정의

- #ifndef로 같은 헤더가 정의되어있지 않은 경우에만 코드를 실행하라
- #define으로 정의
- #endif로 ifndef가 끝났다는것을 알림

### 숙제 1.

- [내 풀이] (https://github.com/JongKyuHong/NBC_TIL/blob/main/3%EC%A3%BC%EC%B0%A8/code/day8Q1.cpp)

### 숙제 2.

- [내 풀이] (https://github.com/JongKyuHong/NBC_TIL/blob/main/3%EC%A3%BC%EC%B0%A8/code/day8Q2.cpp)
- multiply에서 접근 제어 지시자들은 class 단위로 적용되기때문에 other의 멤버변수에 .으로 바로 접근할 수 있다.

## 객제지향 프로그래밍

### 멤버 초기화 리스트

생성자 함수에서 사용할 수 있는 문법

Vehicle(string c, int s) : color(c), speed(s) {}
생성자 내부보다 먼저 실행되고 color를 선언함과 동시에 초기화함

-

* 자식 클래스의 생성자는 부모 클래스의 생성자를 호출 할 수 있다.

### 다형성

- 기본이 되는 클래스를 만들어 함수의 인터페이스를 정의하고 실제 구현은 파생 클래스에서 담당하는 기법
- 동적 바인딩을 통해 실제 호출된 객체의 타입에 따라 적절한 파생 클래스의 함수가 실행되도록 하려면 함수 앞에 virtual 키워드를 붙여야 한다.
- virtual 키워드를 붙이면 가상 함수 테이블이 생성된다.

  - 가상 함수 테이블에는 그림처럼 기본 클래스에서 virtual로 선언된 함수가 파생 클래스에서 재정의된경우 그 위치가 기록되어 동적 디스패치가 가능하다.
  - 동적 디스패치 : 어떤 함수를 실행할지 런타임(프로그램이 돌아가는 도중)에 결정하는 기술
  - 만약에 virtual을 붙이지 않으면 부모의 메서드가 호출된다. 파생클래스의 정보를 알수 없으므로

- 개선 코드

```
#include <iostream>
#include <string>

using namespace std;

class Animal
{
public:
	Animal() {};
	virtual void bark() {};
};

class Lion : public Animal
{
public:
	Lion(string word) : m_word(word) {};
	void bark() {
		cout << "Lion : " << m_word << endl;
	};
private:
	string m_word;
};

class Wolf : public Animal
{
public:
	Wolf(string word) : m_word(word) {};
	void bark() {
		cout << "Wolf : " <<  m_word << endl;
	};
private:
	string m_word;
};

class Dog : public Animal
{
public:
	Dog(string word) : m_word(word) {};
	void bark() {
		cout << "Dog" << " " << m_word << endl;
	}
private:
	string m_word;
};

void print(Animal* animal)
{
	animal -> bark();
}

int main()
{
	Lion lion("ahaaaa!");
	Wolf wolf("ohhhhhh");
	Dog dog("ooooooooooooooops");

	print(&lion);
	print(&wolf);
	print(&dog);

	return 0;
}

```

- ->(화살표) 는 주소(포인터)를 따라가서 멤버를 찾으라는 뜻이고, .(점)은 실제 객체에서 바로 멤버를 찾으라는 차이가 있다.
- virtual을 안붙이면 정적 바인딩, 붙이면 동적 바인딩

### 가상함수

- 인터페이스는 필요하지만 기본 클래스에서 구현할 필요가 없는 경우 순수 가상함수로 선언한다.
- 예시) 사자, 개 모두 특정 울음소리를 가지지만 동물이라는 개념자체에는 특정 울음소리가 없다.
- 가상 함수를 정의하고 =0 을 붙이면 된다.
- 순수 가상 함수를 하나 이상 포함하는 클래스를 추상 클래스라고 한다.
- 추상 클래스는 직접 인스턴스를 생성할 수 없고, 포인터 혹은 참조를 통해서만 사용할 수 있다.

### 숙제 1.

- [내 풀이] (https://github.com/JongKyuHong/NBC_TIL/blob/main/3%EC%A3%BC%EC%B0%A8/code/day8Q3.cpp)

### 1주차 숙제

- [내 풀이] (https://github.com/JongKyuHong/NBC_TIL/blob/main/3%EC%A3%BC%EC%B0%A8/code/day8Q4.cpp)

### 궁금한거 정리

#### 리터럴값과 상수값의 차이?

- 리터럴도 변경할 수 없는 값 그 자체, 상수도 변경할수없는 값인데 둘의 차이가 무엇인가?
- 리터럴 : 이름이 없는 값 그 자체, 코드에 10이라고 적으면 그게 리터럴
- 상수 : 값이 변하지않는 변수(공간)이다. const int maxScore = 100에서 maxScore가 상수, 100은 리터럴

#### 캡슐화와 추상화의 차이?

- 추상화 : "복잡해서" 감추는 것
  - 설계 단계의 개념이자 이 객체가 무엇을 하는지만 보여주고 어떻게 하는지는 몰라도 되게 만듬
- 캡슐화 : "위험해서" 감추는 것
  - 구현 단계의 개념 데이터와 동작을 하나로 묶고, 데이터를 외부로부터 보호

#### 멤버변수를 다룰때 왜 getter, setter를 사용할까?

- 안정성과, 유지보수성을 위해

1. 무결성 보장

- 만약에 멤버변수에 들어가면 안되는값 예시) HP=-500 이런식으로 넣게 되면 오류가 난다 하지만 setter를 통해서 값을 넣게 되면 그에대한 유효성 검사를 할 수 있다.

2. 내부 구현 은닉(유지보수성)
   만약 나중에 HP를 저장하는 방식을 int에서 float로 변경하거나 암호화해서 저장해야 한다면? public변수였다면 이 변수를 사용하는 모든 코드를 다 고쳐야 한다. 하지만 getter/setter를 쓴다면 하뭇 내부만 바꾸면 된다.

#### 가상함수에서 내부를 그냥 구현하지 않은것과 =0붙이는것의 차이?

virtual void func() {} : 일반 가상함수인 경우, 강제성이 없다, 이 클래스로 객체를 만들 수 있다.

virtual void func() = 0 : 순수 가상 함수인경우 강제성이 매우 강하다. 자식 클래스에서 이 함수를 구현하지 않으면 컴파일이 안된다, 이 클래스로 객체를 만들 수 없다.

#### 상속받을때 왜 public으로 받는가

```
class Dog : public Animal {};
```

왜 부모의 클래스를 상속받을때 public을 쓸까? 다른것을 쓰면 안될까?
-> public을 쓰는 이유는 부모에서 공개한 정보(public)은 나도 공개하고 부모에서 protected한것은 나도 하겠다고 선언하는 것이다. 거의 대부분의 경우 public을 사용한다.

#### size_t 자료형

- 가장 큰 양수 정수, 부호가 없는 정수를 나타날때 사용
- 배열의 크기나 메모리 주소계산처럼 절대 음수가 될수없고 아주 클수도 있는 숫자에 사용

#### 소멸자

- 객체(인스턴스)가 죽을 때(메모리에서 사라질때)마지막으로 한 번 호출되는 함수
- 클래스 이름앞에 ~붙임
- 동적할당받았던(new) 메모리 해제(delete)하거나 열어둔 파일 닫는등 뒷정리 청소라고 보면됨
- 객체가 사라지는 시점에 알아서 자동으로 호출

#### virtual ~Adventure() {}

- 가상 소멸자
- 뒷정리청소
- 부모 소멸자에 virtual이 없으면 어떤 파생클래스에서 소멸하는지를 몰라서 부모만 청소한다.
- 부모 소멸자에 virtual이있으면 자식부터 소멸시키고 부모를 소멸시킨다.
