# Net Relevancy

이 Actor 정볼를 이 클라이언트에게 지금 보내줄 필요가 있느냐?
100명의 플레이어가 있는 큰 맵에서 내 위치에서 수 km 떨어진 플레이어까지 매 프레임 복제할 필요는 없다.

중요한건 Relevancy는 클라이언트마다 따로 판단한다. 같은 Actor라도 A 플레이어에게는 Relevant이고, B 플레이어에게는 Not Relevant일 수 있다. 언리얼은 기본적으로 AActor::IsNetRelevantFor()를 통해 특정 Connection에 대해 이 Actor가 Relevant한지를 판단한다.

보통

- 거리
- 소유한 클라만
- 자기 Owner의 판단 따라가기
- 일정 거리안에서
- 거리에 상관없이

등등의 조건이 있다.

```
Actor가 Replication 대상인가?
        ↓
    bReplicates
        ↓
이 Connection에게 보낼 필요가 있는가?
        ↓
    Net Relevancy
        ↓
      Relevant
        ↓
실제로 복제 후보가 됨
```

bReplicated = true라고 해도 Not Relevant면 복제를 하고 있지 않는다.

# Dormancy

이 Actor는 한동안 상태가 안바뀌니까, 아예 복제 검사 대상에서 빼두자
Dormant상태에서 그냥 상태 갱신을 해버리면
예시

```c++
bIsPowered = true
```

새 상태가 바로 복제되지 않을 수 있다. 그래서

```c++
// 상태가 바뀔때
FlushNetDormancy()
bIsPowered = true;

// 계속 변경될 예정이라면
SetNetDormancy(DORM_Awake);
bIsPowered = true;
```

처럼 깨워줘야 한다.

# 연사 무기의 네트워크 설계

나쁜 구조

```
발사
→ RPC
발사
→ RPC
발사
→ RPC
발사
→ RPC
...
초당 12번
```

네트워크가 불안정하면 특히 Reliable RPC를 이렇게 남발했을때 오래된 요청이 밀려 문제가 생길 수 있음

더 좋은 구조

```
마우스 누름
-> ServerStartFile()

서버
-> 자체 Timer
-> 1/12초마다 실제 발사 판정

마우스 뗌
-> ServerStopFire()
```

행동 결과를 매번 전송하는게 아니라 행동의 시작/종료 의도를 전송하는 식으로 구현
그리고 네트워크가 잠깐 끊겼다고 과거 총알들이 우르르 처리되는 구조를 피하는것도 중요하다.

# UObject Subobject

```
Actor Replication
        ↓
Actor 내부 Property 복제


하지만
Actor
 └─ 별도 UObject

는 별도로 '이 UObject도 Replicated Subobject다' 라고 등록해야됨
```

예를들어 캐릭터안에 별도로 무기상태 객체가 있다고 했을때

```c++
UWeaponState* WeaponState;

// 객체안의 값들
int32 Ammo;
float Heat;
bool bJammed;
```

이런식으로 되어있을때 캐릭터자체는 정상적으로 복제되지만 다른 클라이언트에서는 WeaponState내부 값들이 동기화되지 않을 수있다.

우선 캐릭터가

```c++
UPROPERTY()
TObjectPtr<UWeaponState> WeaponState;
```

이렇게 무기를 가질때

```c++
// MyCharacter.cpp

AMyCharacter::AMyCharacter()
{
    bReplicates = true;
    bReplicateUsingRegisteredSubObjectList = true;
}
```

bReplicateUsingRegisteredSubObjectList = true를 켜주고
서버에서 WeaponState를 만들었으면

```c++
WeaponState = NewObject<UWeaponState>(this);

AddReplicatedSubObject(WeaponState);
```

AddReplicatedSubObject로 등록해주기
-> 이 UObject도 Character같이 네트워크 복제해라 라는 뜻
