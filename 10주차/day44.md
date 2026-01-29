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
