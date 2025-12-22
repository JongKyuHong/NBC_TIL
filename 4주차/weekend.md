# C++

## 디자인 패턴

- 개발 시 반복적으로 등장하는 문제를 해결하기 위한 일반화된 솔루션
- 디자인패턴은 크게 3가지로 분리된다.

### 생성 패턴

- 새로운것을 만들어내는 방법과 관련된 패턴
- 공장에서 물건을 찍어내는 것에 비유가능
- 프로그래밍에서는 객체가 된다.
- 싱글톤 패턴

### 구조 패턴

- 여러 부품을 어떻게 조립하고 연결하는 방법에 대한 패턴
- 여러 개의 객체들의 구조를 어떻게 구성할지가 이 패턴의 주 관심사
- 데코레이터 패턴에 대해 알아보자

### 행동 패턴

- 부품이 서로 어떻게 상호작용할지에 대한 패턴
- 특정 객체가 변할 때 다른 객체들이 이 상태를 어떻게 전달할지
- 옵저버 패턴

### 생성패턴(싱글톤 패턴)

> 문제상황

- 비행기 게임을 만듬
- 게임 내 제어하는 비행기는 반드시 하나만 존재
- 비행기 게임을 혼자 하는데 여러 개의 비행기를 제어해야하는 경우는 없다.
- 생성 및 대입을 `private`으로 했다.
- `getInstance()`를 통해서만 인스턴스를 받는다.

```c++
#include <iostream>

using namespace std;

class Airplane {
private:
    static Airplane* instance; // 유일한 비행기 객체를 가리킬 정적 포인터
    int positionX;
    int positionY;

    Airplane() : positionX(0), positionY(0) {
        cout << "Airplane Created at (" << positionX << ", " << positionY << ")" << endl;
    }

public:
    Airplane(const Airplane&) = delete;
    Airplane& operator=(const Airplane&) = delete;

    static Airplane* getInstance() {
        if (instance == nullptr) {
            instance = new Airplane();
        }
        return instance;
    }

    void move(int deltaX, int deltaY) {
        positionX += deltaX;
        positionY += deltaY;
        cout << "Airplane moved to (" << positionX << ", " << positionY << ")" << endl;
    }

    void getPosition() const {
        cout << "Airplane Position : (" << positionX << ", " << positionY << ")" << endl;
    }
};

Airplane* Airplane::instance = nullptr;

int main() {
    Airplane* airplane = Airplane::getInstance();
    airplane->move(10, 20);
    airplane->getPosition();

    Airplane* sameAirplane = Airplane::getInstance();
    sameAirplane->move(-5, 10);
    sameAirplane->getPosition();
}
```

- 왜 레퍼런스를 삭제할까?

```c++
Airplane(const Airplane&) = delete;
Airplane& operator=(const Airplane&) = delete;
```

- 복사본이 아니고 원본을 삭제하기 위해서 자기자신(\*this)를 리턴해야 불필요한 객체복사가 잃어나지 않고 원본 유지가 가능

- 왜 정적 멤버 초기화는 클래스 밖에서 할까?

```c++
Airplane* Airplane::instance = nullptr;
```

- C++의 메모리 할당 시점과 정의 규칙때문에
- 클래스 내부 : 여기는 설계도, 이런 변수가 있을 수 있다 실제로 선언만한 상태
- 클래스 외부 : 여기가 정의부
- 왜 안에서는 못할까?
  - C++ 표준 문법상 static멤버 변수는 클래스 선언부 내부에서 초기화 할 수 없다.

### 구조패턴(데코레이 패턴)

- 피자를 만드려고 함
- 피자의 토핑을 원하는대로 추가하면서 동적으로 피자를 만들 수 있어야 함
- 어떻게 하면 객체의 상태를 동적으로 업데이트 할 수 있을까?

- 기본 피자 클래스가 있고 데코레이터 클래스가 추가로 있다.
- 데코레이터 클래스는 피자 객체를 감싸며 동적으로 새로운 기능을 제공한다.
- 원하는 토핑을 조합하여 피자를 구성할 수 있고, 새로운 토핑을 추가하면 기능 확장이 쉽게 가능하다.

```c++
#include <iostream>
#include <string>

using namespace std;

// **추상 컴포넌트 (Component): Pizza**
// - 피자 객체의 기본 구조를 정의하는 인터페이스입니다.
// - 모든 피자는 이름(`getName`)과 가격(`getPrice`)을 가져야 합니다.
class Pizza {
public:
    virtual ~Pizza() {}
    virtual string getName() const = 0;  // 피자의 이름 반환
    virtual double getPrice() const = 0; // 피자의 가격 반환
};

// **구체 컴포넌트 (Concrete Component): BasicPizza**
// - 기본 피자 클래스입니다.
// - 피자의 기본 베이스(이름과 가격)를 구현합니다.
class BasicPizza : public Pizza {
public:
    string getName() const {
        return "Basic Pizza"; // 기본 피자의 이름
    }
    double getPrice() const {
        return 5.0; // 기본 피자의 가격
    }
};

// **데코레이터 추상 클래스 (Decorator): PizzaDecorator**
// - 기존 피자의 기능을 확장하기 위한 데코레이터의 기본 구조를 정의합니다.
// - 내부적으로 `Pizza` 객체를 감싸며, 이름과 가격에 추가적인 기능을 제공합니다.
class PizzaDecorator : public Pizza {
protected:
    Pizza* pizza; // 기존의 피자 객체를 참조합니다.
public:
    // 데코레이터는 피자 객체를 받아서 감쌉니다.
    PizzaDecorator(Pizza* p) : pizza(p) {}

    // 소멸자에서 내부 피자 객체를 삭제합니다.
    virtual ~PizzaDecorator() {
        delete pizza;
    }
};

// **구체 데코레이터 (Concrete Decorators): Cheese, Pepperoni, Olive**
// - 각각의 토핑 데코레이터는 `PizzaDecorator`를 상속받아 이름과 가격을 확장합니다.

// 치즈 토핑 데코레이터
class CheeseDecorator : public PizzaDecorator {
public:
    CheeseDecorator(Pizza* p) : PizzaDecorator(p) {}
    string getName() const {
        // 기존 피자의 이름에 " + Cheese"를 추가
        return pizza->getName() + " + Cheese";
    }
    double getPrice() const {
        // 기존 피자의 가격에 치즈 추가 비용 1.5를 더함
        return pizza->getPrice() + 1.5;
    }
};

// 페퍼로니 토핑 데코레이터
class PepperoniDecorator : public PizzaDecorator {
public:
    PepperoniDecorator(Pizza* p) : PizzaDecorator(p) {}
    string getName() const {
        // 기존 피자의 이름에 " + Pepperoni"를 추가
        return pizza->getName() + " + Pepperoni";
    }
    double getPrice() const {
        // 기존 피자의 가격에 페퍼로니 추가 비용 2.0을 더함
        return pizza->getPrice() + 2.0;
    }
};

// 올리브 토핑 데코레이터
class OliveDecorator : public PizzaDecorator {
public:
    OliveDecorator(Pizza* p) : PizzaDecorator(p) {}
    string getName() const {
        // 기존 피자의 이름에 " + Olive"를 추가
        return pizza->getName() + " + Olive";
    }
    double getPrice() const {
        // 기존 피자의 가격에 올리브 추가 비용 0.7을 더함
        return pizza->getPrice() + 0.7;
    }
};

// **클라이언트 코드**
// - 피자와 데코레이터를 조합하여 최종 피자를 생성하고, 정보를 출력합니다.
int main() {
    // 1. 기본 피자를 생성합니다.
    Pizza* pizza = new BasicPizza();

    // 2. 치즈 토핑을 추가합니다.
    pizza = new CheeseDecorator(pizza);

    // 3. 페퍼로니 토핑을 추가합니다.
    pizza = new PepperoniDecorator(pizza);

    // 4. 올리브 토핑을 추가합니다.
    pizza = new OliveDecorator(pizza);

    // 5. 최종 피자 정보 출력
    cout << "Pizza: " << pizza->getName() << endl; // 피자의 이름 출력
    cout << "Price: $" << pizza->getPrice() << endl; // 피자의 가격 출력

    // 6. 메모리 해제
    delete pizza;

    return 0;
}
```

### 행동 패턴(옵저버 패턴)

- 엑셀 프로그램을 만들때
- 엑셀 데이터값을 변경할때 해당 데이터로 생성한 모든 차트의 데이터가 반영되게 하고싶다.
- Subject
  - 상태를 관리하고, 변경되었음을 옵저버에게 알림
- Observer
  - `Subject`를 관찰하며 상태 변경 시 반응한다.
- 옵저버들은 서브젝트에 등록하고 변경이 발생하면 알람을 받는다.

```c++
#include <iostream>
#include <vector>
#include <string>
using namespace std;

// Observer 인터페이스
// - Observer 패턴에서 상태 변화를 알림받는 객체들의 공통 인터페이스
// - Observer들은 이 인터페이스를 구현하여 `update` 메서드를 통해 데이터를 전달받음
class Observer {
public:
    virtual ~Observer() = default;               // 가상 소멸자
    virtual void update(int data) = 0;           // 데이터 업데이트 메서드 (순수 가상 함수)
};

// Subject 클래스 (엑셀 시트 역할)
// - 데이터의 상태 변화를 관리하며, 모든 등록된 Observer들에게 변경 사항을 알림
class ExcelSheet {
private:
    vector<Observer*> observers;                 // Observer들을 저장하는 리스트
    int data;                                    // 현재 데이터 상태

public:
    ExcelSheet() : data(0) {}                    // 생성자: 초기 데이터 값은 0

    // Observer 등록 메서드
    // - 새로운 Observer를 등록하여 변경 사항 알림을 받을 수 있도록 추가
    void attach(Observer* observer) {
        observers.push_back(observer);
    }

    // 데이터 변경 알림 메서드
    // - 등록된 모든 Observer들의 `update` 메서드를 호출하여 데이터 변경 사항을 알림
    void notify() {
        for (Observer* observer : observers) {
            observer->update(data);              // 각 Observer에게 데이터를 전달
        }
    }

    // 데이터 설정 메서드
    // - 데이터를 변경하고 변경 사항을 모든 Observer에게 알림
    void setData(int newData) {
        data = newData;                          // 새로운 데이터로 갱신
        cout << "ExcelSheet: Data updated to " << data << endl;
        notify();                                // Observer들에게 알림
    }
};

// 구체적인 Observer 클래스: BarChart (막대 차트)
// - 데이터를 막대 그래프로 표현
class BarChart : public Observer {
public:
    void update(int data) {                      // 데이터 업데이트 시 호출됨
        cout << "BarChart: Displaying data as vertical bars: ";
        for (int i = 0; i < data; ++i) {
            cout << "|";                         // 데이터 값만큼 막대 출력
        }
        cout << " (" << data << ")" << endl;
    }
};

// 구체적인 Observer 클래스: LineChart (라인 차트)
// - 데이터를 선형 그래프로 표현
class LineChart : public Observer {
public:
    void update(int data) {                      // 데이터 업데이트 시 호출됨
        cout << "LineChart: Plotting data as a line: ";
        for (int i = 0; i < data; ++i) {
            cout << "-";                         // 데이터 값만큼 선 출력
        }
        cout << " (" << data << ")" << endl;
    }
};

// 구체적인 Observer 클래스: PieChart (파이 차트)
// - 데이터를 파이 그래프로 표현
class PieChart : public Observer {
public:
    void update(int data) {                      // 데이터 업데이트 시 호출됨
        cout << "PieChart: Displaying data as a pie chart slice: ";
        cout << "Pie [" << data << "%]" << endl; // 데이터 값 출력 (가정: % 비율로 표현)
    }
};

// 메인 함수
int main() {
    // Subject 생성
    ExcelSheet excelSheet;                       // 데이터를 관리하는 엑셀 시트 객체 생성

    // Observer 객체 생성 (각 차트 객체)
    BarChart* barChart = new BarChart();         // 막대 차트 생성
    LineChart* lineChart = new LineChart();      // 라인 차트 생성
    PieChart* pieChart = new PieChart();         // 파이 차트 생성

    // Observer 등록
    // - 각 차트(Observer)를 엑셀 시트(Subject)에 등록
    excelSheet.attach(barChart);
    excelSheet.attach(lineChart);
    excelSheet.attach(pieChart);

    // 데이터 변경 테스트
    // - 데이터를 변경하면 등록된 모든 Observer들이 알림을 받고 화면에 갱신
    excelSheet.setData(5);                       // 데이터 변경: 5
    excelSheet.setData(10);                      // 데이터 변경: 10

    // 메모리 해제
    // - 동적 할당된 Observer(차트) 객체 삭제
    delete barChart;
    delete lineChart;
    delete pieChart;

    return 0;
}
```

- 숙제

```c++
#include <iostream>
#include <vector>

class Animal {
public:
    virtual void speak() = 0;
    virtual ~Animal() = default;
};

class Dog : public Animal {
public:
    Dog() {};
    void speak() {
        std::cout << "멍멍!" << std::endl;
    }
};

class Cat : public Animal {
    void speak() {
        std::cout << "야옹!" << std::endl;
    }
};

class Wolf : public Animal {
    void speak() {
        std::cout << "으르렁!" << std::endl;
    }
};

class Tiger : public Animal {
    void speak() {
        std::cout << "어흥!" << std::endl;
    }
};

int main() {
    std::vector<Animal*> animals;
    animals.push_back(new Dog());
    animals.push_back(new Cat());
    animals.push_back(new Wolf());
    animals.push_back(new Tiger());

    for (const auto& animal : animals) {
        animal->speak();
    }

    for (const auto& animal : animals) {
        delete animal;
    }


    return 0;
}
```

### 언리얼 엔진 기본 개념



### 질문들

1. 순수가상함수랑 그냥 가상 함수 차이가 자식에서 무조건 구현해야된다 아니다 차이인거지?
   -> ㅇㅇ
2. 소멸자는 언제 호출하는거야? 언제는 쓰고 언제는 그냥 생략해서 너무 헷갈림
   -> 객체 생에 끝날때 자동 호출, new로 생성한경우 delete로
3. 생성자가 없는데 어떻게 new Dog()가 가능한거야?
   -> 컴파일러가 기본 생성자 만들어줌
4. 만약에 이 코드에서 Animal클래스에서 소멸자가 없으면 자식이 제대로 메모리 해제되지 않아?
   -> 부모 포인터로 자식을 지울때 문제가 생길 수 있다, 부모 소멸자가 virtual이 아니면 무조건 부모의 소멸자를 호출하기 때문에
5. 자식에서는 왜 소멸자를 안쓴거지?
   -> 필요한 정리작업이 없다. 현재 자식은 리소스를 가지고 있지 않다.
6. Dog dog = new Dog(), Animal dog = new Dog()안되는 이유
   -> new Dog()의 타입은 \*Dog(포인터), Aniaml dog = Dog() 처럼 객체 대 객체로 쓰면 또 다른 문제인 객체 슬라이싱이 생긴다. Dog안에 있는 Dog만의 정보는 잘리고 Animal부분만 복사된다. 이 상태에서 dog.speak()를 호출하면 항상 Animal기준으로만 동작해서 다형성이 깨진다.
   그래서 다형성을 유지하려면 항상 포인터나 레퍼런스 사용해야 한다.
7. 소멸자는 꼭 정의해야 하는가?
   -> 상속 구조 + 다형성 사용할때 부모에서
   -> 클래스가 직접 관리해야할 자원을 가질때, new로 할당한 멤버가 있다던지
8. 왜 new Dog는 포인터인가?
   -> 힙영역에 Dog객체가 들어갈 만큼 메모리할당
   -> 그 메모리 위치에 Dog의 생성자를 호출해서 객체 초기화
   -> 그 메모리 주소 반환
   -> 그래서 반환타입 포인터

## 알고리즘

- [소수의 연속합](https://www.acmicpc.net/problem/1644)
- [내 풀이] (https://github.com/JongKyuHong/NBC_TIL/blob/main/3%EC%A3%BC%EC%B0%A8/code/weekend/1.cpp)

- [소수구하기](https://www.acmicpc.net/problem/1929)
- [내 풀이] (https://github.com/JongKyuHong/NBC_TIL/blob/main/3%EC%A3%BC%EC%B0%A8/code/weekend/2.cpp)

- [프린터 큐](https://www.acmicpc.net/problem/1966)
- [내 풀이] (https://github.com/JongKyuHong/NBC_TIL/blob/main/3%EC%A3%BC%EC%B0%A8/code/weekend/3.cpp)

- [스택 수열](https://www.acmicpc.net/problem/1874)
- [내 풀이] (https://github.com/JongKyuHong/NBC_TIL/blob/main/3%EC%A3%BC%EC%B0%A8/code/weekend/4.cpp)

- [통계학] (https://www.acmicpc.net/problem/2108)
- [내 풀이] (https://github.com/JongKyuHong/NBC_TIL/blob/main/3%EC%A3%BC%EC%B0%A8/code/weekend/5.cpp)

- 소수 출력 부분을 확실하게 기억해야 할듯

- [집합](https://www.acmicpc.net/problem/11723)
- [내 풀이] (https://github.com/JongKyuHong/NBC_TIL/blob/main/3%EC%A3%BC%EC%B0%A8/code/weekend/6.cpp)
