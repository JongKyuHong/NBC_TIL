# 팀프로젝트

## 오늘 작업 내용

### GameManager.cpp 구현

1. showMainMenu : 메인 메뉴
2. createCharacter : 캐릭터 생성
3. startPhase : 페이즈 시작시 몬스터 초기화
4. runBattle : 전투 (실제 전투는 BattleService에서 구현)
5. runBossBattle : 보스전
6. generateMonster : 몬스터 생성
7. generateBoss : 보스 몬스터 생성
8. runShop : 상점
9. showGameOverScreen : 게임오버 시 화면
10. showEndingScreen : 게임 엔딩 시 화면
11. askShopVisit : 상점 방문 여부
12. handlePlayerDeath : 플레이어 사망
13. retryCurrentBattle : 사망 후 전투 재도전 선택 시

### GameManager.h 변경점

1. 페이즈마다 몬스터정보를 담아놓는 phase(1,2,3)Monsters 벡터의 타입을 기존 이름만 담고있던 string에서 이름,정보를 담고있는 구조체 MonsterData 로 변경
2. 전투 재도전시 사용하는 retryCurrentBattle 추가

### BattleService내 BattleLogic설계 및 구현

1. battle : 전투 담당
2. playerTurn : 사용자 턴 진행
3. monsterTurn : 몬스터 턴 진행
4. displayBattleStatus : 사용자와 몬스터의 스텟을 보여줌
5. addLog : 로그 추가
6. displayLog : 전투 로그 출력
7. rollCritical : 크리티컬 확률계산
8. applyCriticalMultiplier : 크리티컬 데미지 적용

### RewardManager구현

- 기존 Reward에서 이름 변경
- 리워드 계산, 제공, 출력담당

1. generateRewards : 리워드 생성
2. applyRewards : 플레이어에게 리워드 제공
3. displayRewards : 획득 리워드 출력

## 오류모음

### 1.

- error C4703: 초기화되지 않았을 수 있는 로컬 포인터 변수 'monsterInfo'이(가) 사용되었습니다.

```c++
Monster* GameManager::generateMonster() {
    std::vector<MonsterData>* monsterInfo; // << 문제 발생 초기화가 되지않아서

    switch (currentPhase) {
    case PhaseType::PHASE_1:
        monsterInfo = &phase1Monsters;
        break;
    case PhaseType::PHASE_2:
        monsterInfo = &phase2Monsters;
        break;
    case PhaseType::PHASE_3:
        monsterInfo = &phase3Monsters;
        break;
    }

    // 현재 라운드에 맞는 몬스터
    MonsterData Info = (*monsterInfo)[currentRound % monsterInfo->size()];
    ...
```

- 컴파일러는 현재 swtich문에서 모든 경우를 처리한다는 보장이 없다고 판단해서 경고를 띄우게 된다.

#### 해결 방법

- nullptr로 초기화 and default케이스를 추가하여 해결하였다.

```c++
Monster* GameManager::generateMonster() {
    std::vector<MonsterData>* monsterInfo = nullptr;

    switch (currentPhase) {
    case PhaseType::PHASE_1:
        monsterInfo = &phase1Monsters;
        break;
    case PhaseType::PHASE_2:
        monsterInfo = &phase2Monsters;
        break;
    case PhaseType::PHASE_3:
        monsterInfo = &phase3Monsters;
        break;
    default:
        monsterInfo = &phase1Monsters;
        break;
    }

    // 현재 라운드에 맞는 몬스터
    MonsterData Info = (*monsterInfo)[currentRound % monsterInfo->size()];
```

### 2.

```c++
enum class PhaseType {
    PHASE_1,
    PHASE_2,
    PHASE_3
};
```

- 게임에 각 페이즈가 있기때문에 이렇게 열거형으로 관리를 하고 있었다.

```c++
// 많은 생략
...
case GameState::CHARACTER_CREATION:
    createCharacter();
    currentState = GameState::PHASE_1;
    break;

case GameState::PHASE_1:
    startPhase(PhaseType::PHASE_1);
    break;
...
// 많은 생략

void GameManager::startPhase(PhaseType phase) {
    if (currentPhase != phase) {
        currentPhase = phase;
        currentRound = 0;

        switch (phase) {
        case PhaseType::PHASE_1:
             std::cout << "변수의 숲" << std::endl;
            phase1Monsters = {
                { "VariableSlime", "값이 변하는 물렁물렁한 슬라임"},
                {"ConstantGolem", "변하지 않는 단단한 바위 골렘"},
                {"TypeSpirit", "데이터의 형태를 결정하는 정령"},
                {"InitWisp", "시작할 때 빛을 내는 위스프"},
                {"DeclareShadow", "실체 없이 이름만 먼저 보이는 그림자"}
            };
            totalRoundsInPhase = 5;
            break;
        case PhaseType::PHASE_2:
             std::cout << "클래스의 성" << std::endl;
            phase2Monsters = {
                {"ClassMimic", "무엇이든 찍어낼 수 있는 상자 괴물"},
                {"ObjectOrc","클래스에서 실체화된 강력한 오크"},
                {"MemberSpider", "객체 내부의 데이터를 지키는 거미"},
                {"FunctionMage", "기능을 수행하고 값을 반환하는 마법사"},
                {"ConstructGargoyle", "객체가 생성될 때 깨어나는 가고일"},
                {"DestructReaper", "객체가 사라질 때 나타나는 사신"}
            };
            totalRoundsInPhase = 6;
            break;
        case PhaseType::PHASE_3:
             std::cout << "디자인 패턴의 탑" << std::endl;
            phase3Monsters = {
                {"SingletonKing", "오직 하나만 존재하는 왕"},
                {"DecoratorKnight", "장비를 덧붙여 강해지는 기사"},
                {"ObserverBeholder", "상태 변화를 감시하는 눈 괴물(비홀더)"},
                {"StrategyGeneral", "상황에 따라 전술을 바꾸는 장군"}
            };
            totalRoundsInPhase = 4;
            break;
        }
    }
    ...
}
```

- 캐릭터를 생성하고, 페이즈1로 넘어가는 당시에 이렇게 클래스의 멤버변수 currentPhase와 phase를 비교해서 각 페이즈에 맞는 몬스터를 초기화 해주고있었는데 문제가 생겼었다.
- currentPhase를 초기화를 해주는 부분이 없어서 startPhase를 캐릭터 생성하고 처음 방문시에 currentPhase != phase가 동작하지 않았다.

#### 해결 방법

```c++
enum class PhaseType {
    NONE,
    PHASE_1,
    PHASE_2,
    PHASE_3
};

class GameManager
{
private:
PhaseType currentPhase = PhaseType::NONE;
...
}
```

- 열거형에 초기상태로 NONE을 넣어주고 currentPhase를 초기화하는식으로 해결하게 되었다.
- 생성자에서 초기화해야하나 생각했지만 우선은 직접 초기화해주는식으로 고치게되었다.

### 3.

```c++

void GameManager::runBattle() {
    Monster* monster = generateMonster();

     std::cout << monster->getName() << "가 나타났다." << std::endl;

    // 전투 전 버프적용, 자동전투한다면 구현
    //applyBuffItems();

    // 실제 전투
    BattleService battleService;
    BattleResult result = battleService.battle(player, monster);

    delete monster;

    // 전투정보 받아와서 처리
    if (result.playerWon) {
        std::cout << "🏆 전투 승리! 🏆\n";
        // mob킬수저장
        // mobKillCounts[monster->getName()]++; << "에러" 메모리 해제후 접근
        mobKillCounts[result.monsterName]++; // result에 추가하여 해결

        // 다음라운드 실행
        currentRound++;
        if (currentRound >= totalRoundsInPhase) {
            currentState = GameState::BOSS_BATTLE;
        } else {
            // 상점에 방문하는가?
            if (askShopVisit()) {
                currentState = GameState::SHOP;
            } else {
                currentState = GameState::BATTLE;
            }
        }
    } else {
        handlePlayerDeath();
    }
}
```

- 전투 로직을 만들고 실제 GameManager에서 전투를 시작하고 결과를 처리하는 부분에서 monster를 메모리해제했는데 mobKillCounts에서 접근하는 오류가있었다.

#### 해결방법

- BattleResult라는 전투결과를 담아놓은 구조체에 monsterName을 추가하여 해결하였다.

### 4.

```c++
int gold = 10;
int exp = 50;

BattleResult* result = new BattleResult(gold, exp);

// BattleResult.h
struct BattleResult {
    bool playerWon;          // 승리 여부
    int goldEarned;          // 획득 골드
    int expEarned;           // 획득 경험치
    std::string monsterName; // 처치한 몬스터 이름
    bool isBossKill;         // 보스 처치 여부
    int totalDamageDealt;    // 플레이어가 준 총 데미지
    int totalDamageTaken;    // 플레이어가 받은 총 데미지
    int turnCount;           // 총 턴 수

    BattleResult(int gold, int exp)
        : playerWon(false), goldEarned(gold), expEarned(exp),
        monsterName(""), isBossKill(false),
        totalDamageDealt(0), totalDamageTaken(0), turnCount(0) {
    }
};
```

- 이런식으로 수정하려고 했는데 `포인터 사용 피하기: C++에서 struct는 값으로 반환하는 게 표준. new/delete 관리 부담 없음.` 라고한다.

#### 해결 방법

```c++
BattleResult(int gold, int exp)
    : playerWon(false), goldEarned(gold), expEarned(exp), monsterName(""), isBossKill(false),
    totalDamageDealt(0), totalDamageTaken(0), turnCount(0) {
} // 생성자 변경

BattleResult result(gold, exp); // 바로 초기화
```

## 코드카타

### 소수만들기

```c++
#include <vector>
#include <iostream>
#include <cmath>
using namespace std;

bool isPrime(int num){
    if (num < 2){
        return false;
    }
    if (num == 2){
        return true;
    }
    for (int i = 2; i <= sqrt(num); i++){
        if (num % i == 0){
            return false;
        }
    }
    return true;
}

int solution(vector<int> nums) {
    int answer = 0;
    for (int i = 0; i < nums.size() - 2; i++){
        for (int j = i+1; j < nums.size()-1; j++){
            for (int k = j + 1; k < nums.size(); k++){
                int sum = nums[i] + nums[j] + nums[k];
                if (isPrime(sum)){
                    answer++;
                }
            }
        }
    }



    return answer;
}
```

- <cmath>헤더에있는 sqrt를 빼먹지 말자
- C++에서 조합 만드는방법을 배워서 써먹으려고 했는데 생각보다 복잡해보여서 우선은 삼중for문으로 해결했다.
