# C++

## 학습가이드 실습

### 미니 실습

#### 실습 내용

- 아래 코드를 보고 출력 결과를 예측
- 예측한 결과에 대한 이유를 설명해주세요.

```c++
#include <iostream>

using namespace std;

void process(int a){
    cout << "Int 버전 호출됨" << endl;
}

void process(char a){
    cout << "Char 버전 호출됨" << endl;
}

int main(){
    short s = 50;
    process(s);

    return 0;
}

```

> short 타입은 int, char중 어디로 변환될까? C++은 더 안전하다고 판단되는 변환을 선택한다.

현재 process 함수는 char와 int형으로 오버로딩 되어있다. main함수에서 short타입변수 s를 넣어주었는데 C++컴파일러는 short타입을 어떤 process함수에 전달할지 결정해야 한다.
변환 규칙은 여러가지가 있는데 어쨌던 short를 int로 변환하는것은 데이터 손실이 없지만 char형으로 변환하는것은 데이터 손실이 있을수있기 때문에 int형으로 변환한다.

### 도전 실습

- 아래는 노션 학습문서의 배열 템플릿 코드스니펫이에요.
- 제공되는 코드에 **인덱스 연산자 []**를 오버로딩하여 구현하세요.
- **[] 연산자**는 **읽기/쓰기** 모두 가능해야 합니다.
- main()에 구현한 연산자를 테스트하기 위해 예제 코드를 작성해주세요.
- **선택 구현:** 배열 범위를 벗어난 인덱스 접근 시 알맞는 예외를 Throw하세요.

> 설계

인덱스 연산자를 operator로 오버로딩해야함, 읽기/쓰기가 가능하려면 참조(T&)를 반환해야 한다. val=arr[0], arr[0] = val 둘다가 가능해야 한다.
예외처리의 경우에는 out_of_range를 사용해서 인덱스가 0보다 작거나 size보다 클때를 방어

```c++
#include <iostream>
#include <stdexcept>

using namespace std;

template <typename T>
class Array {
    T data[100];
    int size;
public:
    Array() : size(0) {}

    T& operator [](int index) {
        if (index < 0 || index >= size) {
            throw out_of_range("Index out of bounds");
        }
        return data[index];
    }

    void add(const T& element) {
        if (size < 100)
            data[size++] = element;
    }

    void remove() {
        if (size > 0)
            size--;
    }

    void print() {
        for (int i = 0; i < size; i++)
            cout << data[i] << " ";
        cout << endl;
    }
};

int main() {
    try {
        Array<int> arr; // 정수형 배열 생성
        arr.add(10);
        arr.add(20);
        arr.add(30);

        // 초기상태
        arr.print();

        // 값 읽기
        cout << arr[0] << endl;

        // 값 쓰기
        arr[0] = 99;
        cout << arr[0] << endl;
        arr.print();

        // 예외 처리
        cout << arr[3] << endl;
    } catch (const out_of_range& e) {
        cerr << "예외 발생! 오류 메시지: " << e.what() << endl;
    }

    return 0;
}
```

### 도전x도전 실습

```c++
// Range-based for loop 라는 문법이에요.
// 보통 foreach라고도 불려요.
for (int num : arr)
{
}
```

C++컴파일러는 for(auto e : arr)같은 for-each문을 만나면 내부적으로는

```c++
{
    auto iter = arr.begin();
    auto end_iter = arr.end();
    for (;iter!=end_iter; ++iter){
        auto e = *iter;
        // 사용자 코드 실행
    }
}
```

이런식으로 동작한다. 따라서 클래스안에 begin()과 end()가 존재해야 foreach문법이 작동한다.

> 왜 포인터(T\*)를 반환할까?

Array클래스는 T data[100]이라는 기본 배열을 사용한다. C++에서 배열의 포인터는 그 자체로 훌륭한 이터레이터 처럼 동작한다. begin(), end()로 주소값만 던져주면 for문이 알아서 주소를 하나씩 증가시키며 데이터에 접근하게 된다.

```c++
...
T* begin() {
    return &data[0];
}

T* end() {
    return &data[size];
}


int main() {
    Array<int> arr; // 정수형 배열 생성
    arr.add(10);
    arr.add(20);
    arr.add(30);

    for (int ar : arr) {
        cout << ar << endl;
    }

    return 0;
}
```

- 내부적으로는 어떻게 도는가?
- [참고](https://cppinsights.io/)

```c++
#include <iostream>
using namespace std;

template<typename T>
class Array
{
  T data[100];
  int size;

  public:
  inline Array()
  : size{0}
  {
  }

  inline void add(const T & element)
  {
    if(this->size < 100) {
      this->data[this->size++] = element;
    }

  }

  inline T * begin()
  {
    return operator&(this->data[0]);
  }

  inline T * end()
  {
    return operator&(this->data[this->size]);
  }

  inline void remove()
  {
    if(this->size > 0) {
      this->size--;
    }

  }

  inline void print()
  {
    for(int i = 0; i < this->size; i++) {
      operator<<(operator<<(std::cout, this->data[i]), " ");
    }

    std::cout.operator<<(std::endl);
  }

};

/* First instantiated from: insights.cpp:38 */
#ifdef INSIGHTS_USE_TEMPLATE
template<>
class Array<int>
{
  int data[100];
  int size;

  public:
  inline Array()
  : size{0}
  {
  }

  inline void add(const int & element)
  {
    if(this->size < 100) {
      this->data[this->size++] = element;
    }

  }

  inline int * begin()
  {
    return &this->data[0];
  }

  inline int * end()
  {
    return &this->data[this->size];
  }

  inline void remove();

  inline void print();

};

#endif

int main()
{
  Array<int> arr = Array<int>();
  arr.add(10);
  arr.add(20);
  arr.add(30);
  {
    Array<int> & __range1 = arr;
    int * __begin1 = __range1.begin();
    int * __end1 = __range1.end();
    for(; __begin1 != __end1; ++__begin1) {
      int ar = *__begin1;
      std::cout.operator<<(ar).operator<<(std::endl);
    }

  }
  return 0;
}

```

## 개념들

### 임의 접근?

- 임의 접근 반복자는 배열처럼 인덱스를 통해 어느 위치든 바로 접근할 수 있는 가장 강력한 반복자

```C++
vector<int> vec = {10, 20, 30, 40, 50};

auto it = vec.begin();
it = it + 3; // +, - 연산 가능
it += 2;  // +=, -= 연산 가능
cout << it[2]; // [] 연산자로 접근 가능
cout << (it < vec.end());// 크기 비교 가능
```

### 범위, 인덱스, 반복자 기반 for문

- 범위 기반 for문 : for (auto x : vec)
- 인덱스 기반 for문 : for(int i = 0; i < N; i++)
- 반복자 기반 for문 : for(auto it = vec.begin(); it!=vec.end(); ++it)

### 반복자 무효화

- 반복자 무효화는 컨테이너의 내용을 변경하는 중에 기존에 가지고 있던 반복자(포인터)가 더 이상 유효하지 않게 되는 현상

```c++
vector<int> vec = {1, 2, 3, 4, 5};

for (auto it = vec.begin(); it != vec.end(); ++it){
    if (*it == 3){
        vec.erase(it); // erase후 it는 무효화
        // ++it하면 크래시 발생
    }
}

// 올바른 코드
for (auto it = vec.begin(); it != vec.end(); ){
    if (*it == 3){
        it = vec.erase(it); // erase는 다음 유효한 반복자를 반환한다.
    } else {
        ++it;
    }
}
```

> 왜 무효화 될까?

vector는 내부적으로 연속적인 메모리 공간을 사용한다. 요소를 삽입하거나 삭제하면 메모리가 재배치될 수 있고, 이 과정에서 기존 반복자가 가리키던 주소가 더 이상 유효하지 않게 된다.

### declared type ( 선언된 타입 )

decltype = declared type
선언된 타입이 무엇인지 알려주는 역할을 한다.

- auto : 값에 맞는 타입을 추론한다(참조나 const가 제거될 수 있다)
- decltype : 식이나 변수의 타입을 그대로 추출 (참조, const모두 보존)

```c++
int x = 10;
decltype(x) y = 20; // y는 int타입 선언

const int& ref = x;
decltype(ref) ref2 = y; // ref2는 const int& 타입으로 선언됨
....
int x = 5;
int& ref = x;

auto a = ref; // int (참조 제거됨)
decltype(ref) b = x; // int& 참조 유지됨
```

## 3번과제

### 설계

1. Inventory<T> 클래스작성
   - [x] 템플릿 클래스로 작성
2. 멤버 변수를 할당한다.
   - [x] T\* pItems\_
     - 동적 배열을 가리키는 포인터
   - [x] int capacity\_
     - 최대 공간
   - [x] int size\_
     - 현재 공간
3. 생성자
   - [x] 매개변수로 용량을 받고, 값을 전달하지 않으면 기본값 10으로 설정한다.
   - [x] 0이하의 값은 1로 보정
   - [x] new T[capacity_]로 힙에 할당
4. 소멸자
   - [x] delete[] pItems\_ 으로 메모리 해제
   - [x] 메모리 해제 후 포인터 nullptr로 초기화
5. void AddItem(const T& item)
   - [x] size\_가 capacity보다 작은경우 아이템 추가, size 1증가
   - [x] 인벤토리가 꽉차면 "인벤토리가 꽉 찼습니다!" 메시지 출력
6. void RemoveLastItem()
   - [x] 실제 메모리를 지우지 않고, 아이템 개수를 나타내는 size\_를 1감소 시킴
   - [x] 인벤토리 비어있으면 "인벤토리가 비어있습니다." 메시지 출력
7. int GetSize() cosnt
   - [x] size\_반환
8. int GetCapacity() const
   - [x] capacity\_ 반환
9. void PrintAllItems() const
   - [x] for문 Index로 0 ~ size\_-1 순회
   - [x] 각 객체의 PrintInfO()함수 호출

```c++
// 정보를 출력하는 멤버 함수
void PrintInfo() const {
    cout << "[이름: " << name_ << ", 가격: " << price_ << "G]" << endl;
}
```

    - [x] 인벤토리가 비어있다면 (비어있음) 출력

### 도전 기능 설계

10. 복사 생성자 및 대입 함수 구현
    - [x] 인벤토리 복사/대입 시 깊은복사 구현
    - [x] Inventory(const Inventory<T>& other); (복사생성자 구현)
      - 객체 생성+초기화 시 호출
    - [x] void Assign(const Inventory<T>& ohter); (대입 역할 함수 구현)
      - 이미 존재하는 객체 값 덮어씌움
11. AddItem 기능 확장
    - [ ] 인벤토리 꽉찬상태로 호출 시 기존 capacity의 2배로 확장하고 새로운 아이템 추가
12. Resize 멤버 함수 추가
    - [x] void Resize(int newCapacity) : 인벤토리의 capacity를 newCapacity로 변경
13. SortItems 멤버 함수 추가
    - [x] void SortItmes() : 가격 오름차순으로 정렬
    - [x] 표준 라이브러리 sort함수 활용, 정렬기준 정의는 compareItemByPrice를 사용

### 구현

- 내일 코드보면서 적기

## C++복습 세션

### 퀴즈 3: 아래 코드는 자식 클래스의 Speak() 함수가 호출되고 있지 않아요. 호출이 잘 되도록 수정해 보세요!

```c++
#include <iostream>

using namespace std;

class Animal {
public:
    void Speak() {
        cout << "Animal sound\n";
    }
};

class Dog : public Animal {
public:
    void Speak() {
        cout << "Woof!\n";
    }
};

int main() {
    Animal* p = new Dog();
    p->Speak();
    delete p;
}
```

- void Pseack에 virtual추가, Dog의 Speak에 override추가
- Speak()함수가 일반 함수이기 때문에 포인터의 타입을 기준으로 함수가 결정된다.
- 정적 바인딩 혹은 컴파일 타임 바인딩이라고 한다.
- virtual을 붙이면 동적바인딩 또는 런타임 바인딩이 되어 실제 객체의 타입을 기준으로 함수가 결정된다.

### 퀴즈 4: 아래 코드는 delete시 메모리에서 해제 되지 않는 문제가 발생해요. 메모리에서 해제가 잘 되도록 수정해 보세요!

```c++
#include <iostream>

using namespace std;

class Base {
public:
    ~Base() {
        cout << "~Base" << endl;
    }
};

class Derived : public Base {
public:
    ~Derived() {
        cout << "~Derived" << endl;;
    }
};

int main() {
    Base\* p = new Derived();
    delete p;
}
```

- 부모의 소멸자를 virtual로 만들면 된다.
- 소멸자가 virtual이 아니면, Base\* 포인터를 기준으로 Base의 소멸자만 호출된다.
