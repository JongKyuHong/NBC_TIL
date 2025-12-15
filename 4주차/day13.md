# 2번 과제하기

## 구현 과정

### 1. 컨트롤 전송으로 다음 초기화를 건너뜀

컨트롤 전송으로 다음 초기화를 건너뜀 이라는 오류가 남

```c++
switch (Inp) {
case 1:
    cout << "원하시는 닉네임을 입력하세요. ";
    string tmpName;
    cin >> tmpName;
    player->setNickname(tmpName);
    break;
```

switch문의 구조상 다른 case로 점프하는 경로가 tmpName초기화를 건너뛸 수 있다고 판단해서 컴파일러가 막은것(string처럼 생성/소멸이 필요한 타입은 더 엄격)

> 해결방법

case 1에 중괄호로 감싸서 tmpName의 생존범위를 중괄호로 제한시켜서 해결했다.

```c++
switch (Inp) {
case 1: {
    cout << "원하시는 닉네임을 입력하세요. ";
    string tmpName;
    cin >> tmpName;

    player->setNickname(tmpName);
    break;
}
```

### 2. 도전 기능 가이드

0

```c++
class Monster{
    public:
        ...
        void attack(Player* player);
        ...
}
// monster.h

class Monster;

class Player{
    public:
        ...
        virtual void attack(Monster* monster) = 0;
        ...
}
// Player.h
```

- 예시의 Player.h에서 Monster클래스를 전방 선언해준것처럼 Monster.h에서도 void attack(Player\* player)를 위해서 class Player라는 빈 클래스를 전방선언해주었다.
- 이는 Player라는 클래스가 어딘가에 정의되어 있으니 미리 알려준다라는 뜻이라고 보면 된다.

### 3. .cpp, .h분리

```c++
// Copyright 2025 JongKyu.

#include "Player.h"
#include "Warrior.h"
#include "Monster.h"
#include <string>
#include <iostream>

using namespace std; // NOLINT

Warrior::Warrior(string nickname) : Player(nickname) {}

void Warrior::attack() {};

void Warrior::attack(Monster* monster) {
    int damage = getPower() - monster->getDefence();
    if (damage <= 0) {
        damage = 1;
    }

    std::cout << " * " << monster-> getName() << "에게 검으로 "
        << damage << "의 피해를 입혔다!\n";

    int TmpHP = monster->getHP() - damage;
    monster->setHP(TmpHP);
    int MonsterHP = monster->getHP();

    if (MonsterHP > 0) {
        std::cout << "몬스터 생존! 남은 HP : " << MonsterHP << "\n";
    } else {
        std::cout << "몬스터 처치 플레이어 승리!!\n";
    }
}

```

- 헤더 파일에는 선언부만 두고 직접 구현은 cpp에서 함, 클래스명::멤버함수 or 생성자로 요구사항에 맞게 행동하게끔 구현했다.

### 4. 함수의 리턴타입으로 포인터 주기

```c++
...
#include <random>

static std::random_device rd;
static std::mt19937 gen(rd());
static std::uniform_int_distribution<> dis_monster(0, 4);

Monster* CreateRandomMonster() {
    string monsterNames[] = {"슬라임", "고블린", "발록", "오크", "드래곤"};
    int index = dis_monster(gen);

    Monster* monster = new Monster(monsterNames[index]);
    return monster;
}

...

Monster* monster = CreateRandomMonster(); // 함수 호출
```

- 몬스터를 랜덤하게 등장시키고 싶어서 함수를 만들었는데 이 과정에서 함수의 리턴타입으로 포인터도 지정가능하다는 것에 대해서 알게 되었다.
- `random`라이브러리를 통해 랜덤값을 만들었다.

### 5. 메모리 해제해주기

```c++
if (player != nullptr)
    delete player;
if (monster != nullptr)
    delete monster;
```

- `new`로 동적할당한 메모리를 프로그램 종료 전에 `delete`로 메모리 해제하여 메모리 누수 방지

# C++공부

### 스마트 포인터

### 순수 가상 함수

`virtual void makeSound() = 0;`

- 내용의 구현없이 함수 선언만 존재하는 것으로 클래스에서 순수가상함수를 선언한 경우 그 클래스는 인스턴스를 생성할 수 없다.
- 자식클래스에서 반드시 생성하도록 강제하는 방법이다.

# 알고리즘

### 1.

- [크리스마스 선물](https://www.acmicpc.net/problem/14235)
- [내 풀이](https://github.com/JongKyuHong/NBC_TIL/blob/main/3%EC%A3%BC%EC%B0%A8/code/day13/1.cpp)
- map의 메서드들을 사용해서 풀이하였다.
- mm이 비어있는지 확인하고 비어있으면 -1
- 문제에서 양의정수만 주어진다고 했기때문에 mm에 값을 저장할때 -1을곱해서 가장 큰값이 begin()으로 오게끔 하였다.
- tmp->second가 0이면 mm.erase(tmp)로 바로 삭제해주었다.

- [개선 풀이](https://github.com/JongKyuHong/NBC_TIL/blob/main/3%EC%A3%BC%EC%B0%A8/code/day13/2.cpp)
- 그냥 우선순위큐 쓰면 된다.
- 처음 써봄
- 기본으로 최대힙이라고 한다.
