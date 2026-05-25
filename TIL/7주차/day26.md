![](https://velog.velcdn.com/images/kyu_/post/c4e9fec7-2a91-4c2c-a45d-be49c01e32f2/image.png)

# 팀프로젝트

## 오늘 작업할 내용

- UIManager로 출력부 분리
- 보스전 만들기
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

### 보스전 만들기

- monster->isBossMonster()로 보스 여부를 판단하고, 일반 전투/보스 퀴즈 전투를 분리했다

```c++
BattleResult BattleService::battle(Character* p, Monster* m) {
    player = p;
    monster = m;

    RewardManager* rewardManager = RewardManager::getInstance();
    BattleResult result = rewardManager->generateRewards(monster);

    uiManager.showBattleStart(monster->isBossMonster());
    uiManager.showMonsterInfo(
        monster->getName(),
        monster->getMobInfo(),
        monster->getHealth(),
        monster->getMaxHealth(),
        monster->getAttack()
    );
    uiManager.waitForKeyPress();

    if (monster->isBossMonster()) {
        return bossBattle();
    } else {
        return normalBattle();
    }
}
```

- normalBattle() (기존 턴제 전투 유지)

  - 기존 while 루프 기반의 플레이어/몬스터 턴 진행 로직은 normalBattle()에 그대로 보존했다.​

- bossBattle() 전체 흐름
  - 보스는 공격하지 않고, 퀴즈 3문제를 랜덤으로 출제한다.​
  - 플레이어는 답만 입력하며, 3/3 정답 시 승리로 처리했다.​

```c++
BattleResult BattleService::bossBattle() {
    uiManager.showBossEncounter(monster->getName());

    std::string bossName = monster->getName();
    auto quizzes = getBossQuizzes(bossName);

    std::random_device rd;
    std::mt19937 rng(rd());
    std::shuffle(quizzes.begin(), quizzes.end(), rng);

    int playerCorrect = 0;
    const int MAX_ROUNDS = 3;

    for (int i = 0; i < MAX_ROUNDS; ++i) {
        uiManager.showQuizRound(i + 1, MAX_ROUNDS);

        std::string question = quizzes[i].first;
        std::string correct  = quizzes[i].second;
        std::string answer   = uiManager.askQuizAnswer(question);

        std::string lowerAnswer  = toLower(answer);
        std::string lowerCorrect = toLower(correct);

        bool isCorrect =
            lowerAnswer.find(lowerCorrect) != std::string::npos ||
            lowerCorrect.find(lowerAnswer) != std::string::npos;

        if (isCorrect) {
            playerCorrect++;
            uiManager.showQuizResult(true, "정답!", playerCorrect, i + 1);
        } else {
            uiManager.showQuizResult(false, "틀렸습니다! 정답: " + correct, playerCorrect, i + 1);
        }

        if (i < MAX_ROUNDS - 1) {
            Sleep(1500);
        }
    }

    auto* rewardManager = RewardManager::getInstance();
    BattleResult result = rewardManager->generateRewards(monster);
    result.playerWon = (playerCorrect == MAX_ROUNDS);
    result.turnCount = MAX_ROUNDS;

    uiManager.showQuizFinalResult(result.playerWon, playerCorrect, MAX_ROUNDS);
    uiManager.waitForKeyPress();

    return result;
}

```

내일은 보스별 퀴즈셋 정의, 입출력 다듬고 자동버전 하나 더 만들면 될 것같다.
PointerLich는 포인터, 팀 프로젝트/폴리 드래곤 보스는 다형성 관련 문제를 낸다.​

### 아스키아트 건드리기

☆오늘의 최고 시간먹는 괴물☆

- 몬스터정보를 한글로 출력하면 감싸고 있는 박스가 밀렸다.

- 수많은 노력의 흔적들 (실제로는 오십장이 넘음)
  ![](https://velog.velcdn.com/images/kyu_/post/6f06de62-bb00-4180-a383-1cb7a608e9a9/image.png)

![](https://velog.velcdn.com/images/kyu_/post/c86e1152-97cd-443b-a64f-8e226b52e375/image.png)

![](https://velog.velcdn.com/images/kyu_/post/ce8bbbd3-5800-4bdd-a309-5ebe0db24adb/image.png)

![](https://velog.velcdn.com/images/kyu_/post/1abdb70e-d548-471f-bfc2-192bc9460124/image.png)

![](https://velog.velcdn.com/images/kyu_/post/db28ecc2-81dd-4179-a2ec-968dd04438ae/image.png)

- 우선 테두리를 없에는 형식으로 바꿨는데 테두리가 없으면 UI가 많이 안좋아보여서 테두리를 추가하고 몬스터의 설명을 빼는식으로 가야할수도 있을것 같다.

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
