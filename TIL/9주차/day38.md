# Collision & Trace

## Collision은 무엇인가??

- 무엇이 무엇과 어떻게 부딪히는지 정의하는 규칙시스템

### 충돌의 두가지 목적

- Query : 트레이스, 오버랩, 라인트레이스 히트 확인 등
- Physics : 실제 물리 시뮬레이션 충돌/반발/리짓바디 등

### Collision Enabled 4종류

- NoCollision : 완전 무시
- Query Only : 트레이스/오버랩 등 '조회'만 됨(가장 자주 사용)
- Physics Only : 물리 충돌만(트레이스에는 안 잡힐 수 있음)
- Query and Physics : 둘 다

### Object Type / Trace Channel / Response구조

- Object Type : "내가 무엇인가"
- Trace Channel : "레이(질의)가 무엇을 검사할 것인가"
- Response : "상대가 왔을 때 내가 어떻게 반응할 것인가"
- Block : 막아서 히트가 발생
- Overlap : 겹치기만
- Ignore : 완전 무시

### Preset(프로파일)과 커스텀 세팅

- Preset은 Object Type + 각 챈러 Response조합을 저장해둔것
- 팀 작업에서는 Preset을 통일해야 디버깅 시간이 줄어든다.

### Simple vs Complex Collision

- Simple : 간단한 콜리전(박스/캡슐/컨백스) -> 빠름
- Complex : 메시 삼각형 기준 -> 정확하지만 비싸다.
- bTraceComplex 옵션이 여기와 연결된다.

### Hit vs Overlap이벤트

- Hit : block기반, 충돌 지점/노말/충돌정보 풍부
- Overlap : Overlap기반, 들어갔다/나왔다 이벤트 중심
- 초보가 제일 많이 실수하는것 : Generate Overlap Events 체크 안함

## Trace

### Trace란?

- 화면/캐릭터/무기 등에서 "보이지 않는 선/형상을 쏴서 충돌을 질의하는 기능

### 라인트레이스 종류

- By Channel : 특정 Trace Channel로 검사
- By Object Type : 특정 Object Type만 검사
- By Profile : Preset(Profile) 이름 기반으로 검사

### Single vs Multi

- Single : 첫 번째로 막힌 것만 필요할 때 (총알, 상호작용)
- Multi : 관통/여러 개 감지(레이저, 다중 타겟, 벽 뒤 판정 등)

### Shape Trace(스피어/캡슐/박스)

- 라인은 정밀하지만 "약간만 빗나가면 안 맞음"
- 스피어는 초보/UX에 좋음(아이템 줍기, 근접 상호작용)

### Sweep vs "한 점(딱 원)" 판정

- Sweep(Start->End) : 길게 캡슐처럼 보일 수 있음(이동하는 구)
- Start == End로 Sweep : 이동 길이 0, 한 지점에 "딱 원(구)" 판정 느낌

### 디버그 시각화

- UWorld Trace + DrawDebugHelpers 조합
- Kismet Trace(UKismetSystemLibrary) Debug 옵션으로 라인/히트 자동 표시 가능

# 수학 기초

## 내적

- 두개의 화살표가 있고, 하나는 플레이어가 바라보는 방향, 하나는 플레이어에서 적을 향하는 방향
- 완전히 같은 방향일때 1, 수직일때 0, 완전히 반대일때 -1

## 외적

- 두 벡터 A, B가있을때 이 두 벡터를 외적하면 A, B모두에게 90도인 새로운 벡터C가 생김

## 행렬

- 행렬의 곱셈은 왼쪽행렬 행과 오른쪽 행렬 열을 마주보는 원소끼리 곱하고 더함, 이 결과값이 결과 행렬의 한칸을 채움
- 행렬은 주로 언리얼에서 변환(Transformation)에 사용

## 좌표계

- 언리얼은 Z-Up 왼손 좌표계 사용
- Local Space : 자기 자신(오브젝트) 모델링, 본 움직임, 로컬 트랜스폼
- World Space : 전역 공간 (모든 Actor의 기준) 위치 계산, 충돌, 빛의 방향
- View (Camera) Space : 카메라 기준 화면 투영, 렌더링 준비

### 좌표계의 수학적 전환

- 모든 변환은 4x4 동차 좌표 행렬로 수행
- 점과 벡터 구분위해 4번째 성분 w를 추가
  - 점 : [x, y, z, 1][x, y, z, 1][x, y, z, 1] , w = 1 -> 평행 이동의 영향을 받음
  - 벡터 : [x, y, z, 0][x, y, z, 0][x, y, z, 0], w = 0 -> 회전,스케일의 영향만 받음

### 변환 체계

- Local -> World -> View -> Clip좌표로 변환되는 과정
- Local -> World : Mworld(변환행렬), 오브젝트의 위치, 회전, 스케일
- World -> View : Mview, 카메라(관찰자) 기준으로 변환
- View -> Clip : Mproj , 3D를 2D 화면으로 투영

## 행렬의 구조

World Matrix (Object->World)

- P : Parents
- T : Translation
- R : Rotation
- S : Scale

View Matrix (World -> Camera)

카메라의 위치와 방향을 반영한 행렬
"세상을 카메라가 바라보는 방향으로 회전시키고, 카메라 원점으로 이동시킨다."

Projection Matrix(Camera->Clip)

- 카메라의 FOV, Near/Far Plane, Aspect Ratio를 반영하여 3D좌표를 2D화면으로
