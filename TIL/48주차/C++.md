# sort 주의점

```
문제 1 / 6
게임 인벤토리에서 아이템을 희귀도가 높은 순서대로 정렬하려고 해.
struct Item
{
    int id;
    int rarity;
};

std::vector<Item> items =
{
    {1, 5},
    {2, 3},
    {3, 5},
    {4, 1}
};

std::sort(items.begin(), items.end(),
    [](const Item& a, const Item& b)
    {
        return a.rarity >= b.rarity;
    });


개발자는 희귀도가 높은 아이템을 앞으로 보내려고 >=를 사용했어.

질문
1. 희귀도가 같은 아이템 두 개를 비교하면 현재 함수는 어떤 결과를 반환할까?
2. 이 비교 방식이 std::sort에서 문제가 될 수 있는지, 된다면 왜 그런지 설명해 봐.
3. 희귀도가 높은 순서대로 정렬하면서 문제를 방지하려면 어떻게 수정해야 할까?
정확한 문법보다 왜 문제가 생기는지를 이해하고 설명하는 게 중요해.
```

sort할때 조건에 문제가 없다고 생각을 했는데 오류가 하나 있고, 내가 잘못 생각하고 있는 부분도 있었다.

우선 sort에서 람다에서 인자로 받는 a,b는 a가 무조건 앞, b가 무조건 뒤쪽원소가 아니라고 한다.
a > b가 true면 a는 b보다 앞에 와야 한다는 뜻, false면 a가 b보다 앞서야 한다는 조건이 성립하지 않는다 라는 뜻, 또 >= 를 사용하면 동률에서도 true가 나와서 정렬 비교 규칙을 위한해서 Undefined Behavior가 된다고 한다.

std::sort의 비교함수에는 Strict Weak Ordering(엄격 약순서)라는 조건이 있는데, 가장 기본적인 조건은 자기 자신을 비교하면 반드시 false여야 한다는 조건이다.
위 문제에서는 >= 대신 >를 사용하면 되고, 희귀도가 같은 아이템들의 기존 상대 순서까지 유지해야 한다면 std::stable_sort를 사용해야 한다.

std::vector<Enemy> enemies =
{
{1, true},
{2, true},
{3, false},
{4, true}
};

for (size_t i = 0; i < enemies.size(); ++i)
{
if (enemies[i].isDead)
{
enemies.erase(enemies.begin() + i);
}
}

# 함수 이름 숨김(Name Hiding)

```c++
#include <iostream>
#include <string>

class Character
{
public:
    void Attack(int damage)
    {
        std::cout << "Damage: " << damage << '\n';
    }
};

class Player : public Character
{
public:
    void Attack(const std::string& skillName)
    {
        std::cout << "Skill: " << skillName << '\n';
    }
};

int main()
{
    Player player;

    player.Attack("Fire");
    player.Attack(100);
}
```

개발자는 Character에 이미 Attack(int)가 있으니까 Player에서도 두 함수를 모두 사용할 수 있다고 생각했어.

그런데 두 번째 호출에서 컴파일 오류가 발생해.

질문

1. Player가 Character를 상속받았는데도 player.Attack(100)이 컴파일되지 않는 이유는 무엇일까?
2. Player에서 두 종류의 Attack()을 모두 사용할 수 있게 하려면 어떻게 수정해야 할까?

이번에는 상속 구조에서 함수 이름을 탐색하는 방식을 이해하고 있는지 확인하는 문제

---

부모의 Attack(int)를 못찾는다고 한다.
C++은

1. Player에서 Attack이라는 이름을 먼저 탐색
2. Player::Attack(string)을 찾으면 탐색을 멈춤
3. 이때 부모의 Character::Attack(int)는 이름이 숨겨져서 오버로드 후보에 들어가지 않음
4. 찾은 Attack(string)에는 정수 100을 전달할 수 없으므로 컴파일 오류가 발생한다.

```c++
class Player : public Character
{
public:
    using Character::Attack;

    void Attack(const std::string& skillName)
    {
        std::cout << "Skill: " << skillName << '\n';
    }
};
```

using을 사용해서 부모의 같은 이름 함수도 사용할 수 있게 해야한다.
오버라이딩이 아니고 이름 숨김 문제
