# atomic

가장 헷갈리는 부분
atomic은 락을 거는것이 아니고 원자적 연산을 보장해주는것
atomic은 연산을 쪼개지지 않게 보장 + 여러 코어에서 그 atomic값의 변경을 일관되게 관찰하도록 함
atomic을 안써도 캐시 일관성 프로토콜은 동작을 한다. 하지만 두 스레드가 하나의 변수에 대해서 읽고 둘다 값을 쓰는 경쟁상태까지는 막아주지 못한다.

여러 변수 사이의 상태 일관성은 자동으로 묶어주지 않는다.

```c++
struct PlayerState
{
    int HP = 100;
    bool IsDead = false;
};
```

// Thread A

```c++
HP -= 100;

if (HP <= 0)
    IsDead = true;
```

// Thread B

```c++
if (!IsDead)
{
    // 살아있는 플레이어라고 판단하고
    // 어떤 행동을 수행
}
```

여기서 HP, IsDead를 atomic으로 바꾸어도 이 문제는 해결되지 않는다.

ThreadA작업중에

```c++
HP -= 100;        // HP = 0

// 이 사이에 Thread B가 실행될 수 있음

IsDead = true;
```

---

mutex를 써서 해결할 수 있다.

```c++
struct PlayerState
{
    int HP = 100;
    bool IsDead = false;
    std::mutex Mutex;
};
```

// Thread A

```c++
{
    std::lock_guard lock(Mutex);

    HP -= 100;
    if (HP <= 0)
        IsDead = true;
}
```

// Thread B

```c++
{
    std::lock_guard lock(Mutex);

    if (!IsDead)
    {
        // 행동
    }
}
```
