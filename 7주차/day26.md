# 팀프로젝트

## 오늘 작업할 내용

- UIManager로 출력부 분리
- 아스키아트 적용해보기

## 오늘 작업한 내용

### GameManager분리 -> UIManager도입

기존 `GameManager`는 다양한 역할을 가지고 있었다.

- 게임 상태 머신 제어
- 페이즈 진입 화면, 엔딩 화면, 사망 화면등 다양한 cout출력
- clearScreen()같은 화면 제어도 포함

`UIManager`를 도입하여 화면 출력코드를 UIManager로 이동시켰다.
`BattleService`에 있는 출력문도 UIManager에서 담당하기로 하였다.

### 입출력 방식 통일

전투 메뉴 선택같은곳에서는 \_getch를 사용하고, 캐릭터 이름 설정에서는 cin을 사용했어서 모두 Enter를 눌러야하는 cin으로 통일했다.

## 오류모음

### 1. 보스전 스킵 문제

- 해당 페이즈의 모든 일반 몬스터를 잡고나서 해당 페이즈의 보스몬스터와 싸우게 되는데 스킵되는 버그가있었다.
- 보스몬스터쪽은 현재 텍스트만 출력하고 별다른 로직이없는데 텍스트 자체가 출력이 안되었다.
- 확인해보니 이벤트마다 `clearScreen`함수로 콘솔창을 비우게 되는데 이때 보스몬스터를 처치하고나서 나오는 메시지가 지워지고 다음 페이즈로 넘어가서 안보이는거였다.
- `clearScreen`위치를 수정해서 해결하였다.

### 2. 박스 UI정렬

- `playerName`이나 `monsterName`에 따라서 테두리를 동적으로 변경해주어야 했다.

- 기존 한글이름
  ![](https://velog.velcdn.com/images/kyu_/post/bfb87fb2-a10f-4b25-ac3f-f51a5b634c97/image.png)

- 기존 영어이름
  ![](https://velog.velcdn.com/images/kyu_/post/afc27d6e-310a-49e5-9def-397efa1836c3/image.png)

```c++
const int BOX_WIDTH = 60;

int displayWidth = 0;

// 플레이어 이름의 표시 너비 계산
for (size_t i = 0; i < playerName.length(); ) {
    unsigned char c = playerName[i];

    if ((c & 0x80) == 0) {
        // ASCII (영문/숫자) - 1칸
        displayWidth += 1;
        i += 1;
    } else {
        // 한글 등 멀티바이트 - 2칸
        displayWidth += 2;
        // UTF-8에서 한글은 3바이트
        i += 3;
    }
}

// "의 턴" 추가: 의(2칸) + 공백(1칸) + 턴(2칸) = 5칸
displayWidth += 5;

int totalPadding = BOX_WIDTH - displayWidth;
int leftPadding = totalPadding / 2;
int rightPadding = totalPadding - leftPadding;

std::cout << "╔════════════════════════════════════════════════════════════╗\n";
std::cout << "║" << std::string(leftPadding, ' ') << turnTitle << std::string(rightPadding, ' ') << "║\n";
std::cout << "╚════════════════════════════════════════════════════════════╝\n";

std::cout << "\n⚔️  " << playerName << "의 공격!\n";
std::cout << "→ " << monsterName << "에게 " << damage << " 데미지!\n";

if (isMonsterDead) {
    std::cout << "→ " << monsterName << "을(를) 쓰러뜨렸다!\n";
}
```

- 변경 후
  ![](https://velog.velcdn.com/images/kyu_/post/b6ff3f07-4590-4d0b-80f8-242705247602/image.png)
  ![](https://velog.velcdn.com/images/kyu_/post/3bbc8777-792c-4578-ba86-0036524c9630/image.png)

- 이모지를 제거하고 한글은 따로 처리를 해서 해결하였다.
