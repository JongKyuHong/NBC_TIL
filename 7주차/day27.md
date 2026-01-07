# 팀 프로젝트

## 오늘 할일

- 보스전 만들기
- 도전과제 완료
- 시간 남으면 아스키아트 보스전부터 순차 적용

## 오늘 한거

### 1. 보스전 만들기 and 과제 완성

### 2. BattlePhaseScene 완성

- 정리

```
📁 MonsterEncounter + 퀴즈 전투 → 단일 클래스 통합
✅ 등장 애니메이션 (PointerLich 4프레임)
✅ 몬스터 정보 타이핑 효과
✅ 퀴즈 전투 시스템 (문제+선택지)
✅ 깔끔한 레이아웃 구성

```

#### 애니메이션 속도 버그

🐛 애니메이션 속도 조절 버그 해결

```cpp
// ❌ 틀린 방법 (Render에서 매번 업데이트)
void Render() {
    animTimer += 0.016f;  // 매 프레임 리셋 → 속도 조절 불가!
}

// ✅ 올바른 방법 (Update에서만)
void Update(float dt) {
    animTimer += dt;  // 실제 경과시간만 누적
    monsterFrame = (int)(animTimer / FRAME_DURATION) % 4;
}
```

#### 문제/선택지 파싱

```c++
// BattleService에서 문제 작성
"포인터는?\n1) 값\n2) 주소\n3) 타입"

// 자동 파싱
std::vector<std::string> lines;
for (char c : fullText) {
    if (c == '\n') {
        lines.push_back(line);
        line.clear();
    }
}
// lines[0] = 문제, lines[1-3] = 선택지
```

## 오류 모음

### 1.

![](https://velog.velcdn.com/images/kyu_/post/a2c53999-a7d6-4621-b538-38f28f0f5a5b/image.png)

- 퀴즈로 보스전이 진행되는데 첫 라운드에는 Round출력과 문제 출력이 함께되었는데 두번째 라운드부터는 라운드만 출력되고 꼭 엔터를 눌러야 문제가 출력이 되었다.
- 별거 아닌 부분이지만 고치려고 했는데 도무지 원인파악이 안되었다
- ai한테 물어봐도 cout버퍼 문제인것같다고만 한다

- 해결?

```c++
std::string UIManager::askQuizAnswer(const std::string& question) {
    std::cin.clear();
    std::cin.ignore((std::numeric_limits<std::streamsize>::max)(), '\n');
	// 위 두 코드가 문제

    std::cout << "\n";
    std::cout << "╔════════════════════════════════════════════════════════════╗\n";
    std::cout << "   " << question << "\n";  // 질문: 스페이스 3개
    std::cout << "╚════════════════════════════════════════════════════════════╝\n";
    std::cout << "💭 답변: ";

    std::string answer;
    std::getline(std::cin, answer);

    size_t start = answer.find_first_not_of(" \t\n\r\f\v");
    if (start == std::string::npos) return "";
    size_t end = answer.find_last_not_of(" \t\n\r\f\v");
    answer = answer.substr(start, end - start + 1);

    return answer.empty() ? "pass" : answer;
}
```

- 버퍼정리를 할때가 문제였던것 같다 해당 코드를 지우고 출력부아래에 최소의 버퍼만 삭제하게끔 수정하니 해결되었다.
- 이렇게하니 다른 문제들이 쏟아져나왔다. 그냥 cin을 사용한 벌을 받는다 생각하고 엔터눌러서 넘어가자

## 코드카타

### 기사단원의 무기

```c++
#include <string>
#include <vector>
#include <cmath>

using namespace std;

int prime(int num){
    int ans = 0;
    for (int i = 1; i <= sqrt(num); i++){
        if (num % i == 0){
            ans++;
        }
    }
    return ans+1;
}

int solution(int number, int limit, int power) {
    int answer = 0;

    for (int i = 1; i <= number; i++){
        int res = prime(i);
        if (res > limit){
            answer+=power;
        } else {
            answer += res;
        }

    }

    return answer;
}
```

- 기존 방법은 그냥 소수를 구하는 방법을 사용했는데 number가 10만이기때문에 최악의 경우 100억번 연산이 필요했다.

```c++
#include <string>
#include <vector>

using namespace std;

int solution(int number, int limit, int power) {
    vector<int> CountPrime(number+1, 0);

    for (int i = 1; i <= number; i++){
        for (int j = i; j <= number; j+=i){
            CountPrime[j]++;
        }
    }

    int answer = 0;
    for (int i = 1; i <= number; i++){
        if (CountPrime[i] > limit){
            answer += power;
        } else {
            answer += CountPrime[i];
        }
    }

    return answer;
}
```

- 에라토스테네스의 체 방식으로 코드를 개선하여 풀게되었다.
