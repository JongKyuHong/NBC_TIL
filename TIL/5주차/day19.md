## 정리

### 알고리즘 함수

- accumulate : numeric헤더에 포함
- min_element, sort, find, count_if등 알고리즘 : algorithm헤더
- isdigit : cctype

### 형변환

- 암시적 형변환 : 컴파일러가 자동으로 타입을 변환(int->double, char\*->string), 데이터 손실이나 의도치않은 동작을 유발할 수 있다.
- explicit : 클래스 생성자 앞에 붙여 암시적 변환을 막음
- 명시적 형변환
- static_cast : 일반적인 타입 변환(숫자 타입간, 상속 관계 업/다운 스케일링)에 사용, 컴파일 타임에 체크
- dynamic_cast : 상속 관계에서 안전한 다운캐스팅에 사용 런타임에 타입을 체크하며 실패시 'nullptr' 반환(가상 함수 필수)

### 메모리 관리

- memcpy : C스타일 메모리 복사 string, vector같은 내부상태(포인터, 크기 등)가 있는 클래스에 사용하면 메모리누수, 이중해제, 크래시등이 발생한다.
  - 포인터 멤버가 있는 구조체에 사용 시 '얕은 복사'가 일어남

### 객체 지향 프로그래밍, 디자인 패턴

- 생성자/소멸자 : 생성은 부모->자식, 소멸은 자식->부모 순
- 가상소멸자 : 부모 포인터로 자식 객체를 delete할때 자식 소멸자가 호출되도록 보장하는 것, 부모의 소멸자에 virtual을 붙여줌, 메모리 누수 방지
- super : C++ 표준 키워드는 아니며, UE5등에서 부모 클래스를 가리키는 매크로, C++에서는 부모클래스명::함수()로 호출

### SOLID원칙

- SRP : 단일 책임 원칙, 한 클래스는 하나의 책임
- OCP : 개방/폐쇄, 확장에는 열리고 수정에는 닫혀있어야함
- LSP : 리스코프 치환, 자식 클래스는 부모 클래스로 대체 가능해야 함
- ISP : 인터페이스 분리, 범용 인터페이스보다 구체적인 여러 인터페이스가 낫다.
- DIP : 의존 역전 , 구체적인 클래스가 아닌 추상화(인터페이스)에 의존

### 주요 디자인 패턴

- 싱글톤 : 인스턴스가 단 하나만 존재하도록 보장, 전역변수의 편리함을 가지면서, 생성과 수명 관리를 통제할 수 있다.

  - 효용성 : 유일성 보장, 전역접근, 메모리 절약
  - 주의점 : 의존성 숨김(누가 어디서 이 객체를 쓰는지 파악하기 어려움), 전역 상태를 가지므로 단위테스트시 격리가 어려움

- 데코레이터 : 객체 코드를 수정하지 않고 런타임에 기능을 겹겹이 추가, 객체에 동적으로 새로운 책임을 추가하는 패턴
  - 효용성 : 유연한 확장, SPP준수
  - 주의점 : 코드 복잡도, 초기 설정 번거로움
- 옵저버 : 한 객체의 상태가 변하면 의존하는 다른 객체들에게 자동으로 알림, 이벤트 기반 시스템의 핵심, 게임업적/UI시스템 등
  - 효용성 : 느슨한 결합(플레이어 클래스는 UI나 업적시스템을 몰라도 됨), 실시간 반응, 확장성
  - 주의점 : 순서 보장 안됨(알림 받는 순서 보장 어려움), 메모리 누수 (옵저버 해제를 제대로 안하면 좀비 객체가 될 수 있다.)

### 싱글톤 구현

```c++
#include <iostream>

template<typename T>
class Singleton {
public:
    static T& GetInstance() {
        static T instance;
        return instance;
    }

    Singleton(const Singleton&) = delete;
    Singleton& operator=(const Singleton&) = delete;
protected:
    Singleton() {}
    virtual ~Singleton() {}
};

class GameManager : public Singleton<GameManager> {
    friend class Singleton<GameManager>;

private:
    GameManager() {
        std::cout << "GameManager 생성" << std::endl;
        Score = 0;
    }
public:
    int Score;
    void AddScore(int value) { Score += value; }
};

int main() {
    GameManager::GetInstance().AddScore(100);

    std::cout << "Current Score:"
        << GameManager::GetInstance().Score << std::endl;

    GameManager& gm1 = GameManager::GetInstance();
    GameManager& gm2 = GameManager::GetInstance();

    if (&gm1 == &gm2) {
        std::cout << "두 객체는 완벽히 동일한 인스턴스입니다." << std::endl;
    }

    return 0;
}
```

- `static T& GetInstance()` : 전역 접근점
- `static T instance` : 함수 내부에 선언된 static변수는 프로그램 실행시 생성되고, 프로그램 종료될때까지 살아있음
  - 즉 처음 호출시에만 instance가 생성되고, 이후 호출시 이미 생성된 객체를 반환해준다.
  - T& 즉 참조를 반환해주므로 객체의 복사본이 아닌 원본 자체를 반환해서 외부에서 값을 수정하면 원본이 바뀐다.
- `protected`로 생성자와 소멸자를 감싸서 이 클래스를 상속받는 자식클래스에서는 호출할 수 있지만, 외부에서는 직접 생성할 수 없다.
- `public Singleton<GameManager>` : GameManager가 Singleton을 상속받습니다.
  - CRTP가 사용된다. 자기 자신의 타입을 부모 템플릿의 인자로 넘겨준다.
  - 이로써 부모인 Singleton은 자식 타입이 GameManager라는것을 컴파일 타임에 알게 되고, GetInstance()에서 static GameManager instance;를 만들 수 있습니다.
- `friend` 선언 : C++에서 private 멤버는 오직 자기자신만 접근할 수 있습니다.
  - 하지만 Singleton<GameManager>::GetInstance() 함수 안에서 GameManager의 private 생성자를 호출해야 합니다 (static T instance; 부분)
  - friend선언은 너에게만 내 private멤버에 대한 접근을 허락할게 이런 뜻

## 코드카타

### string에서 알고리즘, 이터레이터 쓰기

```c++
long long solution(long long n) {
long long answer = 0;
string newN = to_string(n);
sort(newN.begin(), newN.end(), greater<char>());
    return stoll(newN);
}
```

- 정수 내림차순으로 배치하기 문제는 정수n의 각 자릿수를 내림차순 정렬하고 그를 반환하는 문제였다.
- 이때 sort알고리즘과 이터레이터를 사용할 수 있다.
- string은 내부적으로 char배열을 관리하므로 가능하다.
- 내림차순으로 쉽게 변경할때 greater<T>()를 사용하도록 하자

### 수의 범위

```c++
int solution(int num) {
    int answer = 0;
    long long tmp = num;
    if (num == 1){
        return 0;
    }
    while (true){
        if (tmp % 2 == 0){
            tmp /= 2;
        } else {
            tmp = (tmp * 3) + 1;
        }
        answer++;
        if (answer == 500){
            return -1;
        }
        if (tmp == 1){
            break;
        }
    }
    return answer;
}
```

- 콜라츠 추측문제를 푸는데 num이 int형이라서 수의 범위때문에 올바른 값이 리턴이 되지않았다.
- long long에다가 넣어주니 해결되었다. 문제의 조건을 보고 수의 범위를 생각하면서 해야될듯

### find활용방법

```c++
#include <algorithm>

string solution(vector<string> seoul) {
    string answer = "";
    auto it = find(seoul.begin(), seoul.end(), "Kim");
    answer = "김서방은 " + to_string(it-seoul.begin()) + "에 있다";
    return answer;
}
```

- 서울에서 김서방 찾기문제를 풀때 처음에 vector의 find활용방법이 헷갈렸다.
- 이번기회에 한번 정리해야겠다.

- vector, list는 altorithm헤더의 find를 쓴다.
  - find(vec.begin(), vec.end(), 찾는값);
  - 못찾으면 vec.end()반환
- 멤버 함수 find() : 보통 set, map, multimap, multiset, unordered_set, unordered_map에서 사용
  - map.find(키 혹은 값)
  - 못찾으면 map.end()반환
- string의 find
  - str.find("값");
  - 못찾으면 string::npos반환, 찾아도 다른애들은 반환타입이 이터레이터인데 얘는 size_t

### 벡터에서 가장 작은 값 찾기

```c++
#include <algorithm>

using namespace std;

vector<int> solution(vector<int> arr) {
    auto it = min_element(arr.begin(), arr.end());
    arr.erase(arr.begin() + distance(arr.begin(), it));
    if (arr.empty()){
        arr.push_back(-1);
    }
    return arr;
}
```

- 알고리즘 헤더의 min_element를 활용하면 된다.
- 가장 작은 값의 이터를 반환해주고 distance를 통해서 인덱스를 찾고 해당 인덱스의 값을 삭제해준다.

### 문자열 <-> 문자 변환, substr등

```c++
string solution(string s) {
    int len = s.length();
    string tmp;
    int index = s.length() / 2;
    if (len % 2){
        tmp = s.substr(index, 1);
    } else {
        tmp = s.substr(index-1, 2);
    }

    return tmp;
}
```

- 가운데 글자 가져오기 문제

### 숫자변환

```c++
#include <cctype>

bool solution(string s) {
    if (s.length() == 4 || s.length() == 6){
        for (int i = 0; i < s.length(); i++){
            if (!isdigit(s[i])){
                return false;
            }
        }
        return true;
    } else{
        return false;
    }
}
```

- isdigit은 cctype헤더에 있다.
