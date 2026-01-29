# 8번과제

## 데이터 에셋 만들기

- PerkDataAsset.h을 만듬
- EPerkRarity : 특성 등급을 정의, 지금 뽑힌게 무엇인지 아는 역할
- FPerkInfo : 개별 특성 정보 담기
- FPerkRollResult : 뽑힌 등급과 랜덤으로 뽑힌 개별 특성 목록
- FPerkRarityValues : 특성등급 배율 넣어놓음

## GAS

- 캐릭터의 스텟을 관리
- GameEffect와 연결하려고 함

## PerkManager

- 단순히 어떤 등급을 뽑을지 랜덤값 돌리기
- 특성 3개 뽑기 역할

# 챌린지반

## GAS

- Attribute : 캐릭터의 수치 데이터
- AttributeSet : Attribute를 관리하는 클래스 (C++)
- ACS : Ability System Component(GAS의 중심)

### 직접 수정방식의 문제점

- 네트워크 동기화 : 수동으로 복체처리 필요
- 데미지 계산 : 직접 계산해야됨
- 재사용성 : 같은 코드를 매번 작성
- 추적 : 어떤 변화가 일어났는지 추적하기 힘듬
- 버프/디버프 : 타이머로 수동 관리

## Gameplay Effect

- 위의 문제들을 해결
- Attribute를 변경하는 방법을 정의한 Blueprint or Class
- 게임플레이 이펙트는 블루프린트에서 하는게 편하고 좋다

### 역할

- Attribute변경
- Tag 부여/제거 (상태 표시)
- 버프/디버프 적용
- 상태이상
- 쿨다운 (Ability 재사용 제한)

### 왜씀 그래서??

- 네트워크 복제 : 자동
- 데미지 계산 : Modifier로 정의
- 재사용 : 한번 만들고 재사용 가능
- 추적 : Active Effect확인
- 버프/디버프 : Duration으로 자동
- 조건 : Tag Query로 간단
- 디버깅 : showDebug로 확인

### Effect의 세가지 타입

- Instant : 즉시 적용, 적용후 제거, 데미지/힐
- Duration : N초동안 적용, N초후 자동 제거, 주기설정 가능, 버프/디버프, 독 등
- Infinite : 무한히 적용, 자동제거 안됨, 주기 설정 가능, 패시브, 장비 스텟 등

### Modifier : Attribute를 어떻게 변경할까??

- Add : 더하기
- Multiply : 곱하기
- Divide : 나누기, 이동속도 / 2 , 디버프(느려짐)
- Override : 덮어쓰기, Health = 100(완전회복), 초기화/리셋

### Magnitude Calculation (변화량 계산)

- Scalable Float (가장 간단)
  - 고정 값, 레벨에 따라 스케일링 가능
  - -10.0 : 항상 10데미지
  - 30.0 : 항상 30회복 등
  - 대부분의 Effect에서 사용

- Attribute Based
  - 다른 Attribute의 값을 참조하여 계산
  - 비율 계산 가능
    - MaxHealth × 0.1
    - AttackPower × 0.5
  - 비율데미지, 비율 회복, 스텟 기반 계산

- Custom Calculation Class
  - C++클래스로 복잡한 계산
  - `GameplayEffectExecutionCalculation`상속
  - 실전 데미지 계산, 복잡한 로직

### Period (주기적 적용)

- 독데미지 같은거
- 0이면 한번 적용
- 0보다 크면 그 주기마다 반복
- Instant는 주기없음

### Stacking (중첩)

- 같은 Effect가 여러번 적용될때 동작 정의
- None : 중첩 가능, 독지역에 두번들어가면 두번적용
- Aggregate by Source : 소스별 1개, 적 A의 독, 적B의독 따로
- Aggregate by Target : 타겟당 1개, 독1개만 (새로걸면 갱신, 타겟 즉 맞는사람 기준)

# 코드카타

## 배달

- [문제](https://school.programmers.co.kr/learn/courses/30/lessons/12978)

```c++
#include <iostream>
#include <vector>
#include <queue>
#include <iostream>

using namespace std;

void dijkstra(vector<vector<pair<int, int>>>& vec, vector<int>& dist){
    priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> pq;
    dist[1] = 0;
    pq.push({0, 1});
    while (!pq.empty()){
        int d = pq.top().first;
        int v = pq.top().second;
        pq.pop();

        if (dist[v] < d) continue;

        for (auto& node : vec[v]){
            int next = node.first;
            int nextDist = node.second;

            int newDist = d+nextDist;
            if (newDist < dist[next]){
                dist[next] = newDist;
                pq.push({newDist, next});
            }
        }
    }

}

int solution(int N, vector<vector<int> > road, int K) {
    int answer = 0;

    vector<vector<pair<int, int>>> vec(N+1);

    for (const auto& r : road){
        int start = r[0];
        int end = r[1];
        int d = r[2];

        vec[start].push_back({end, d});
        vec[end].push_back({start, d});
    }

    vector<int> dist(N+1, 500001);

    dijkstra(vec, dist);

    for (int i = 1; i < dist.size(); i++){
        if (dist[i] <= K){
            // cout << i << endl;
            answer++;
        }
    }

    return answer;
}
```

- 오랜만에 만난 다익스트라 문제
- 시작점이 1번이라고 있고, 다른 모든 정점까지 가는 최단거리를 구하기 때문에 다익스트라를 썼다.
- dist벡터를 초기화할때 문제조건에서 K가 50만까지라고 해서 50만+1로 설정했다. (원래는 무한대로 설정)
- 거리를 계속해서 갱신하며 최단거리를 찾는다.
