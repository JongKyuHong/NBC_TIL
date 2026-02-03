# 과제 해설

## 헤더에서 초기화

- UPROPERTY로 블루프린트에서 수정값이 들어가는거는 헤더에서 초기화해도 무방하다.
- 만약 프로젝트 크기가 커지면 헤더에 뭐가 많이들어갈수록 컴파일 시간이 늘어나기 때문에 지양해야됨
- 하지만 작은 규모에서는 충분히 사용해도 된다.

## 게임인스턴스

- 게임 인스턴스는 게임 끝날때까지 없어지지 않으므로
- 데이터 테이블을 게임인스턴스에서 불러와서 사용해도 됐을거같다.

## 서브 시스템

- 게임 만들때 만들었던 Manager들이다.
- 월드에 붙어있냐, 캐릭터에 붙어있냐 인스턴스에 붙어있냐 어디 붙어있는지에 따라 생명주기가 달라진다.

## 게임 스테이트

- 정말 상태만을 관리하게 하면 좋다.
- 나같은 경우 많은 책임을 State에 넣고 분리하지 않았다.

## Static함수

- GameMode나 GameState, GameInstance, PlayerController에서 Static함수를 하나 만들어서 불러오는식으로 하면 편하다.
- 모든 UObject는 부모가 존재.
- UOBject를 통해서 어느 월드에 속해있는지 알 수 있다.

## WBP

- WBP는 팔레트에서 불러서 배치할 수 있다.
- 재사용 가능
- 가능하면 User Widget에 들어가는 코드들은 별도에서 외부에서 하는거 보다는 내부에서 제어하는데 코드 관리가 좋다??

# 내 과제

- 기존 온라인 강의에서 확장한 버전
- 동전을 먹으면 레벨업하는 로직을 추가하였다.
- 레벨업을 하면 캐릭터의 능력을 올려주는 증강을 고를 수 있음
- 아이템은 큰 동전, 작은 동전, 힐 포션, 지뢰, 독 포션, 슬로우 포션이 있음

## 메인 화면

<img width="3817" height="2018" alt="스크린샷 2026-02-02 192905" src="https://github.com/user-attachments/assets/614452b4-f671-4a86-9d6b-b09b42e8c4db" />

## 레벨업

- 동전을 일정량 먹으면 레벨업
- 증강을 고를 수 있음
- 증강은 실버, 골드, 프리즘이라는 등급이 나뉨
  - 등급이 높을 수록 좋은 효과

### 실버 증강

<img width="2647" height="1509" alt="스크린샷 2026-02-02 193016" src="https://github.com/user-attachments/assets/24d52876-54e4-4ecf-b501-e8a06068bd61" />

### 골드 증강

<img width="2645" height="1507" alt="스크린샷 2026-02-02 193056" src="https://github.com/user-attachments/assets/d87956ff-2f39-4b1f-98be-96137579afc2" />

### 프리즘 증강

<img width="2631" height="1501" alt="스크린샷 2026-02-02 193604" src="https://github.com/user-attachments/assets/d762301a-a7af-4492-8592-36a6ecf4aca1" />

## 증강 종류

### 점프력 증가

<img width="749" height="1502" alt="스크린샷 2026-02-02 193548" src="https://github.com/user-attachments/assets/d3873f5a-085e-44a4-a7d0-92625b0549b6" />

### 크기 증가

<img width="746" height="1498" alt="스크린샷 2026-02-02 193553" src="https://github.com/user-attachments/assets/59deb800-4ac0-4c94-8c2c-bdc96867ce88" />

### 이동속도 증가

<img width="746" height="1495" alt="스크린샷 2026-02-02 193558" src="https://github.com/user-attachments/assets/210a2e97-e624-48d1-8052-d21f96ae6e00" />

### 경험치 획득률 증가

<img width="749" height="1506" alt="스크린샷 2026-02-02 193647" src="https://github.com/user-attachments/assets/ac5850c8-bb4d-4044-ba5a-3b0fae3276ef" />

### 최대 체력 증가

<img width="746" height="1507" alt="스크린샷 2026-02-02 193735" src="https://github.com/user-attachments/assets/3254b3bb-06ea-4774-a862-600f9c054b48" />

## 2웨이브

### 슬로우 포션 추가 (좌 상단)

<img width="3771" height="1977" alt="스크린샷 2026-02-02 193041" src="https://github.com/user-attachments/assets/b4b4c436-c098-4a44-b04c-3cd66982bc9c" />

### 슬로우 상태이상 (좌 중단)

<img width="3796" height="2002" alt="스크린샷 2026-02-02 193112" src="https://github.com/user-attachments/assets/f5a368d5-66df-41a7-bdd0-ec4c5dc20c15" />

## 3웨이브

### 독 포션 추가 (좌 상단)

<img width="3293" height="1733" alt="스크린샷 2026-02-03 103257" src="https://github.com/user-attachments/assets/1f546ba9-9523-4407-b262-36e986e6c8a4" />

### 중독 상태이상 (좌 중단)

<img width="3789" height="1977" alt="스크린샷 2026-02-02 193124" src="https://github.com/user-attachments/assets/77338f4c-c2f2-423e-bf0c-ff0e6450bf0f" />

## 시연 영상

[![시연 영상](https://img.youtube.com/vi/loPRwaXU62g/0.jpg)](https://www.youtube.com/watch?v=loPRwaXU62g)

## 과제를 마치며

- 책임에 따라서 분리를 잘하자
- state, mode, instance, controller에 static함수 사용하는거 공부해보자
- 라이라 샘플 프로젝트 분해해서 공부해보자
- readme를 작성할때 프로젝트 구조를 항상 같이 첨부하자

# 코드카타

## 하노이의 탑

```c++
#include <string>
#include <vector>

using namespace std;

void move(int start, int end, int mid, int n, vector<vector<int>>& answer){
    if (n == 1){
        answer.push_back({start, end});
        return;
    }
    move(start, mid, end, n-1, answer);
    answer.push_back({start, end});
    move(mid, end, start, n-1, answer);
}

vector<vector<int>> solution(int n) {
    vector<vector<int>> answer;
    move(1, 3, 2, n, answer);
    return answer;
}
```

- [문제](https://school.programmers.co.kr/learn/courses/30/lessons/12946?language=cpp)
- 하노이의 탑 문제
- 탑이 3개로 고정이기 때문에 start, end, mid (anther가 더 잘어울리나?) 시작점, 도착점, 제3탑을 재귀로 풀면 된다.
- 오랜만이라 좀 많이 당황..
