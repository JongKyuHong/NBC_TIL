# Replication Graph

Replication Graph는 서버가 클라이언트마다 복제할 Actor를 결정할 때, 매번 모든 Actor를 검사하지 않도록 Actor를 미리 분류하고 필요한 복제 후보를 효율적으로 수집하는 네트워크 복제 시스템이다.

---

## 언제 필요한가?

100명의 플레이어가 있는 오픈월드 서버에 Actor가 20,000개 있다고 가정하자.

- 기존 접근: 플레이어마다 20,000개 Actor를 확인하고 Relevancy 등을 판단한다.
- 문제점: 각 플레이어 주변에는 수백 개만 필요해도 후보를 찾는 검사 자체가 비싸다.
- 개선 목표: 무작정 전부 검사하기 전에 검사할 후보 목록을 좁힌다.

Relavancy는 이 Actor가 이 클라이언트에게 필요한가? 를 판단하는 개념, 반면 Replication Graph는 판단할 Actor후보를 어떻게 효율적으로 모을까? 에 초점을 둔다.

## 어떻게 작동하는가?

대표적인 방법은 공간분할이다.

```
서버의 복제 대상 Actor
        │
        ▼
Replication Graph에서 미리 분류
        │
        ├── GridSpatialization2D (위치 기반)
        │      ├── A구역: 몬스터·상자 목록
        │      ├── B구역: 몬스터·NPC 목록
        │      └── C구역: 몬스터·상자 목록
        │
        └── AlwaysRelevant (위치 무관)
               └── GameState 등

플레이어가 B구역에 있다면
→ B구역 및 필요한 인접 구역의 Actor 목록 수집
→ AlwaysRelevant 목록도 함께 수집
→ 후보에 대해 복제 조건·우선순위·갱신 시점 등을 처리
```

**중요** : Replication Graph가 Actor를 무조건 전송하는것은 아니다. 먼저 효율적으로 후보 목록을 구성하고, 이후 실제 복제 여부·빈도·우선순위 등이 처리된다.

## 플레이어가 이동하면?

플레이어의 새 위치를 기준으로 필요한 격자 목록을 가져온다. 이동하는 Actor 역시 위치에 맞게 공간 관리 정보가 갱신되어야 한다. 그래서 20,000개 전체를 매번 검사하는 대신 주변 Actor를 중심으로 처리할 수 있다.

## 자주 나오는 노드

- UReplicationGraphNode_GridSpatialization2D : 2D 격자 기반으로 Actor를 분류하고 공간별 후보 소집, ex) 주변 몬스터, 상자, NPC
- UReplicationGraphNode_AlwaysRelevant : 위치와 무관하게 필요한 Actor를 전역 목록등으로 관리, ex) GameState등
- UReplicationGraphNode_AlwaysRelevant_ForConnection : 특정 연결에 항상 필요한 Actor 목록, ex) 해당 연결의 PlayerController, ViewTarget
- UReplicationGraphNode_DynamicSpatialFrequency : 이동하는 Actor의 거리 등에따라 복제 빈도 조절을 지원, ex) 동적 Actor의 거리 기반 갱신 정책

> AlwaysRelevant는 복제 후보에서 위치 때문에 제외하지 않는다는 뜻이지, 모든 Actor를 매 프레임 반드시 전송한다는 뜻은 아니다.

## 다른 개념과 구분하기

- Relevancy : 이 Actor가 이 플레이어에게 필요한가?
- NetUpdateFrequency : 복제 갱신을 얼마나 자주 시도하는가?
- Dormancy : 변경이 거의 없는 Actor의 복제를 잠시 쉬게 할 수 있는가?
- Replication Graph : 복제할 Actor 후보 목록을 어떻게 효율적으로 찾고 관리하는가?

## Octree와의 차이

- Octree / Uniform Grid : 주변 객체를 효율적으로 찾는 공간 분할 자료구조
- Replication Graph : Actor를 연결별로 효율적으로 복제하기 위해 여러 규칙과 Actor 목록을 관리하는 시스템
- GridSpatialization2D : Replication Graph 안에서 사용하는 2D 격자 기반 노드, Octree처럼 공간을 재귀적으로 8개로 나누는 방식과는 다르다.

## 주의점

- 격자 크기와 주변 탐색 범위가 너무 크면 후보가 많아지고, 너무 작으면 관리 비용이 커질 수 있다.
- 이동하는 Actor의 위치 갱신, 멀리 떨어진 대형 Actor, 층이 겹치는 3D공간등은 별도로 고려해야한다.
- Replication Graph를 써도 대역폭과 복제 데이터량이 저절로 해결되는 것은 아니다.
