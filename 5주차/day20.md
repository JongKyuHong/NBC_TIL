![](https://velog.velcdn.com/images/kyu_/post/eb0a7b58-e877-46c7-b15b-6f247f190929/image.png)

# C++ 복습 세션 점프업

## 생성패턴 : 싱글톤

- 게임매니저나 오디오 매니저 처럼 세상에 단 하나만 존재해야 하는 관리자 녀석들이 필요할때 쓴다.

Q4. `static` 멤버 변수를 사용하는 이유는?

- 객체 생성 없이도 접근 가능하고, 모든 객체가 공유하기 위해

Q5. 싱글톤의 단점으로 지적되는 것은?

- 전역 상태를 만들기 때문에 결합도(Coupling)가 높아질 수 있다.

코드 Q5. `GameManager::instance`를 클래스 밖에서 `nullptr`로 초기화해주지 않으면 어떻게 되는가?

- static변수는 Class밖에서 초기화해주어야한다. 클래스 내부에서 초기화하면 클래스가 생성될때마다 초기화 될 위험이 있어서 컴파일 에러를 뱉는다.

## 구조 패턴 : 데코레이터

- 게임에서의 무기 속성부여 강화시스템 느낌??

Q2. 상속 대신 데코레이터를 쓰는 이유는?

- 기능을 정적으로 고정하지 않고, 실행 중에 동적으로 유연하게 추가/제거하기 위해

Q3. 데코레이터 클래스는 왜 Weapon을 상속받으면서 동시에 Weapon멤버 변수를 가질까?

- 자기도 Weapon처럼 취급되면서(상속), 실제 행동은 감싸고 있는 Weapon에게 위임하기 위해

Q5. 데코레이터 패턴의 단점

- 작은 객체들이 너무 많이 생겨서 디버깅이 조금 복잡해질 수 있다.

## 행동 패턴 : 옵저버

- 구독과 좋아요!
- 주체(Subject)의 상태가 변하면 이를 구독(Observe)하고 있던 관찰자들에게 자동으로 알려주는 패턴
- 게임 UI(체력바 갱신)나 업적 달성에 필수적

Q1. 옵저버 패턴은 주로 어떤 관계를 해결하기 위해 사용하는가?

- 1대다(1:N)의 의존관계에서 데이터 변경 시 자동 갱신

Q3. 옵저버 패턴의 장점은

- Subject와 Observer가 서로 깊게 알필요가 없다(느슨한 결합)

# 코드 카타

## 최소 공배수, 최대 공약수

```c++
int gcd(int a, int b){
    while (b!=0){
        int tmp = b;
        b = a % b;
        a = tmp;
    }
    return a;
}

long long lcm(int a, int b){
    return (long long)a*b / gcd(a, b);
}
```

- 자주까먹는 놈
- C++17부터는 numeric헤더를 통해 만들 수 있다고 한다.

```c++
#include <numeric>

int gcd_result = std::gcd(a, b);
int lcm_result = std::lcm(a, b);
```

- 편리하다!

## 범위

별거는 아닌데 int형이 넘을거같으면 long long을 사용하도록 하자 int의 범위 넘는거 계산안하고 계속 쓰다가 오류난적이 많음

## 백준

오후에 컨디션이 쏘배드라서 알고리즘 풀었다.
예전에 풀던것들을 C++로 풀어보니까 C++에 좀 익숙해지기도 한거같고, 확실히 속도가 빨라서 좋다.
string, vector, queue, map, set 등등 메서드 자주까먹는데 날잡고 한번 외워야 될듯

- [절대값 힙](https://www.acmicpc.net/submit/11286)

  - [내 풀이](https://github.com/JongKyuHong/NBC_TIL/blob/main/%EC%BD%94%EB%93%9C%EC%B9%B4%ED%83%80/day20/%EC%A0%88%EB%8C%80%EA%B0%92%ED%9E%99.cpp)

- [최대 힙](https://www.acmicpc.net/problem/11279)

  - [내 풀이](https://github.com/JongKyuHong/NBC_TIL/blob/main/%EC%BD%94%EB%93%9C%EC%B9%B4%ED%83%80/day20/%EC%B5%9C%EB%8C%80%ED%9E%99.cpp)

- [나는야 포켓몬 마스터 이다솜](https://www.acmicpc.net/problem/1620)

  - [내 풀이](https://github.com/JongKyuHong/NBC_TIL/blob/main/%EC%BD%94%EB%93%9C%EC%B9%B4%ED%83%80/day20/%EB%82%98%EB%8A%94%EC%95%BC%ED%8F%AC%EC%BC%93%EB%AA%AC%EB%A7%88%EC%8A%A4%ED%84%B0%EC%9D%B4%EB%8B%A4%EC%86%9C.cpp)

- [최소 힙](https://www.acmicpc.net/problem/1927)
  - [내 풀이](https://github.com/JongKyuHong/NBC_TIL/blob/main/%EC%BD%94%EB%93%9C%EC%B9%B4%ED%83%80/day20/%EC%B5%9C%EC%86%8C%ED%9E%99.cpp)

# 총 정리

- 디자인패턴 3종 : 싱글톤, 데코레이터, 옵저버
- GCD/LCM : numeric
- 범위 주의
- STL 익히기
