![](https://velog.velcdn.com/images/kyu_/post/b30ea651-5583-445c-8c68-34f649bd695b/image.png)

# Unreal C++강의

## Build.cs

```c++
PublicDependencyModuleNames.AddRange(new string[] {
    "Core",
    "CoreUObject",
    "Engine",
    "InputCore",
    "EnhancedInput",
    "UMG"
    });

PrivateDependencyModuleNames.AddRange(new string[] {  });
```

- 프로젝트에서 필수적으로 사용하는 엔진 기능들
  - Core : 엔진 주요기능
  - CoreUObject : 리플렉션 시스템
  - Engine : 게임엔진의 주요기능
  - InputCore : 입력시스템
  - EnhancedInput : 인핸스드 인풋 시스템
  - UMG : Unreal Motion Graphics의 줄임말 HUD쓰려면 필요

## HUD

- 플레이어 컨트롤러에서 아래를 추가하였다.

```c++
UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="UI")
TSubclassOf<UUserWidget> HUDWidgetClass;

UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "HUD")
UUserWidget* HUDWidgetInstance;

UFUNCTION(BlueprintPure, Category = "HUD")
UUserWidget* GetHUDWidget() const;
```

### HUDWidgetClass

- 어떤 UI를 만드는지 결정하는 클래스 정보
- 붕어빵 틀
- TSubclassOf<UUserWidget>은 유저 위젯의 자식 클래스중 하나를 담겠다는 뜻

### HUDWidgetInstance

- 실제 객체
- 게임안에 실제로 태어난 UI 객체의 주소를 저장

```c++
if (HUDWidgetClass)
{
    HUDWidgetInstance = CreateWidget<UUserWidget>(this, HUDWidgetClass);
    if (HUDWidgetInstance)
    {
        HUDWidgetInstance->AddToViewport();
    }
}
```

- CreateWidget으로 생성된 결과물을 이 변수에 담아둔다.

### GetHUDWidget()

```c++
UUserWidget* AMyPlayerController::GetHUDWidget() const
{
    return HUDWidgetInstance;
}
```

- 외부에서 이 UI인스턴스에 접근하게 해주는 함수

### 초기화

- BeginPlay끝에서 밑 코드를 찾을 수 있다.

```c++
AMyGameState* MyGameState = GetWorld() ? GetWorld()->GetGameState<AMyGameState>() : nullptr;
if (MyGameState)
{
    MyGameState->UpdateHUD();
}
```

- 이는 플레이어 컨트롤러에서 AddToViewport로 Instance정보를 띄워주고 한번 초기화해주는 작업이다.
- 이 코드가 없으면 초기값설정해놓은게 잠깐 보일 수가 있어서 실제 게임 정보로 바로 초기화 해주는것

## 파티클

### SpawnEmitterAtLocation

- 이펙트를 소환할 때 가장 자주 사용하는 함수
- 특정 위치에 파티클을 딱 한번만 생성하고 싶을때 사용한다.

```c++
Particle = UGameplayStatics::SpawnEmitterAtLocation(
    GetWorld(), // 파티클이 소환될 세계
    PickupParticle, // 헤더에서 UParticleSystem*으로 지정한 파티클 에셋
    GetActorLocation(), // 어디에 소환할지
    GetActorRotation(), // 어떤 회전값을 가지고 소환될지
    true // 파티클 재생이 끝나면 메모리에서 자동으로 지울지 결정
);
```

# 특강

## CI/CD

### Content Management

- Package Project : 프로젝트 빌드 전체를 진행
- Cook Content : 리소스 쿠킹만 진행
  - (Resource Cooking : 게임에 사용하기 최적화 된 형태로 가공)

### Binary Configuration

- Use Project Setting
- Development : 개발 환경으로 빌드, 디버그 최적화 당함(변수 값이 잘 안보임) => 잘 안쓰임
- DebugGame : 디버그 환경으로 빌드
- Shipping : 출시 환경으로 빌드

## 디버깅 팁 및 Profiling

### check

- 조건이 거짓이면 치명적으로 중단(Crash)시켜서 이상태가 나오면 안된다 강제

- check : false면 중단, 절대 발생하면 안되는 전제를 강제
- checkSlow : 이거 느리다라고 사람이 알아보기 쉽게 표시
- checkf : check와 같고, 로그로 남김
- checkfSlow : 느린거같은거 + 로그
- checkNoEntry() : 해당 줄이 실행되면 즉시 중단, 여기는 절대 오면 안되는 분기라고 체크해주는것
- checkNoReentry() : 해당 코드지점이 실행중인데 또 호출되었을때 즉시 중단, 재진입 금지를 강제
- checkNoRecursion : 스코프를 빠져나오기전에 같은줄이 다시 hit되면 즉시 중단, 재귀/재호출 금지를 강제
- unimplemented() : 실행되면 즉시 중단, 오버라이드 돼야하는 가상함수나 미구현 경로를 표시

### verify

- 가장 치명적
- check와 기본 동작이 같고 check가 꺼진 빌드에서도 표현식을 평가
- verify(Expression)
- verifySlow(Expression)
- verifyf(Expression, FormattedText, ...)
- verifySlow(Expression, FormattedText, ...)

### ensure

- 조건이 거짓이어도 보통 즉시 중단하지 않고, 로그 / 콜스택을 남기고 계속 진행(가능하면)
- 이상 상태는 맞는데 당장 죽이기에는 큰 문제가 아닌 경우 (비치명적)
- 디버깅 상태에서는 중단점을 잡고, F5를 누르면 넘어감
- ensure(Expression)
  - false면 크래시 리포터에 알림 + 계속 실행
  - 세션당 1회
- ensureMsgf(Expression, FormattedText, …)
  - ensure와 같고, FormattedText를 로그 메시지에 출력
- ensureAlways(Expression)
  - false면 매번 보고 + 계속 실행
- ensureAlwaysMsgf(Expression, FormattedText, …)
  - ensureAlways와 같고, FormattedText를 로그 메시지에 출력

### F5 vs Ctrl + F5

- F5 즉 디버거를 붙여서 실행하면 check를 만난상태에서 거짓이 뜨면 거기서 멈춤
  - F5를 누르면 일단 진행은 됨
- Ctrl + F5 즉 디버거를 안붙이고 실행하면 check를 만난상태에서
  - 그냥 멈추고 죽음 Crash뜨면서

### Profiling Macro Functions

- 함수안에 넣어서 그 함수가 얼마나 걸리는지 확인
- Stat으로 찾기힘들때

1. DECLARE_STATS_GROUP(GroupText, Group, Category)
   - 형태 : 선언
   - 측정하고 싶은 사이클(시간) Stat의 Group을 정의
2. DECLARE_CYCLE_STAT(StatText, StatId, Group)
   - 형태 : 선언
   - 측정하고 싶은 사이클(시간) Stat의 ID를 정의
3. SCOPE_CYCLE_COUNTER(StatId)
   - 형태 : 스코프
   - 명령어 : Stat <Group>
   - 해당 스코프의 실행 시간을 누적하여 측정
4. QUICK_SCOPE_CYCLE_COUNTER(UndefinedStatId)
   - 형태 : 스코프
   - 명령어 : Stat Quick
   - 해당 스코프 실행 시간(간편 정의)을 측정

### 여러가지 팁

- GC호출시기 제어할수는 있는데 바꾸지는 말자
- 안바꾸는데 더 낫다. 여러가지 문제가 야기됨

# 코드카타

## 행렬 테두리 회전

- [문제](https://school.programmers.co.kr/learn/courses/30/lessons/77485)
- 항상 모든 꼭짓점의 값을 저장하고 돌려주었는데
- 시작점 값을 저장하고 역순으로 돌리는게 더 효율적이라고 판단하여 그렇게 풀게되었다.

```c++
int rotate(vector<vector<int>>& vec, vector<int>& query){
    int r1 = query[0]-1;
    int c1 = query[1]-1;
    int r2 = query[2]-1;
    int c2 = query[3]-1;

    int tmp = vec[r1][c1];
    int minV = tmp;

    for (int i = r1; i < r2; i++){
        vec[i][c1] = vec[i+1][c1];
        minV = min(minV, vec[i][c1]);
    }

    for (int i = c1; i < c2; i++){
        vec[r2][i] = vec[r2][i+1];
        minV = min(minV, vec[r2][i]);
    }

    for (int i = r2; i > r1; i--){
        vec[i][c2] = vec[i-1][c2];
        minV = min(minV, vec[i][c2]);
    }

    for (int i = c2; i > c1 + 1; i--){
        vec[r1][i] = vec[r1][i-1];
        minV = min(minV, vec[r1][i]);
    }

    vec[r1][c1+1] = tmp;
    return minV;
}
```

## 두큐합같게만들기

- [문제](https://school.programmers.co.kr/learn/courses/30/lessons/118667)
- 처음에는 dfs를 이용해서 모든 경우를 다 트래킹하였는데 시간초과가 났다.
- 입력을 보면 queue1, queue2의 길이가 30만이라서 많이 길기도 하고, 실제로 큐의 값을 맞추는 과정이기때문에 값이 큰 큐에서 작은큐로 넣는 그리디하게 푸는게 낫다고 판단하여 그렇게 풀게되었다.

```c++
#include <string>
#include <vector>
#include <deque>
#include <iostream>

using namespace std;


int solution(vector<int> queue1, vector<int> queue2) {
    int answer = 0;
    deque<int> q1;
    deque<int> q2;
    long long sum1 = 0;
    long long sum2 = 0;
    for (int i = 0; i < queue1.size(); i++){
        sum1+=queue1[i];
        q1.push_back(queue1[i]);
    }
    for (int i = 0; i < queue2.size(); i++){
        sum2+=queue2[i];
        q2.push_back(queue2[i]);
    }

    int countMax = (queue1.size() + queue2.size())*2;

    while (sum1 != sum2 && answer < countMax){
        // cout << "sum1 : " << sum1 << endl;
        // cout << "sum2 : " << sum2 << endl;
        // cout << "answer : " << answer << endl;
        if (sum1 > sum2){
            int value = q1.front();
            q1.pop_front();
            q2.push_back(value);
            sum1 -= value;
            sum2 += value;
        } else {
            int value = q2.front();
            q2.pop_front();
            q1.push_back(value);
            sum1 += value;
            sum2 -= value;
        }
        answer++;
    }


    return answer == countMax ? -1 : answer;
}
```

## 무인도여행

- [문제](https://school.programmers.co.kr/learn/courses/30/lessons/154540)
- 전형적인 bfs문제

```c++
deque<pair<int, int>> dq;
dq.push_back({i, j});
while (!dq.empty()){
    pair<int, int> current = dq.front();
    // auto [r, c] = dq.front();
    dq.pop_front();
    int r = current.first;
    int c = current.second;
}
```

- 저렇게 pair로 정의해서 뽑을 수도 있고
- auto로 뺄수도있다고 한다.

## 자주쓰는 메서드들 정리하기

### Vector

벡터에서 값을 특정 인덱스에 저장할때
vec.insert(vec.begin() + index, value);

벡터에서 여러 값을 특정 인덱스에 저장할때
vec.insert(vec.begin() + index, count , value);

벡터에서 특정인덱스 값을 삭제할때
vec.erase(vec.begin()+index);

벡터에서 범위삭제
vec.erase(vec.begin() + start, vec.begin()+end); // start부터 end-1까지 삭제

전체삭제
vec.clear();

요소접근
vec[index] || vec.at(index); : at()은 인덱스 범위를 넘어가면 에러를 뱉어줌

가장 앞
vec.front();
가장 뒤
vec.end();
미리 자리 예약
vec.reserve();

### String

뒤에 문자열 주가
s += "abc"; || s.append("abc");

맨뒤에 문자하나 추가
s.push_back('a');

특정 위치에 삽입
s.insert(index, "abc");

특정 위치 문자 교체
s.replace(pos, len, "abc"); : pos부터 len만큼을 abc로 변경한다.

특정 범위 삭제
s.erase(index, len); : index부터 len개만큼 삭제

마지막 문자 삭제
s.pop_back();

전체 삭제
s.clear();

문자열 찾기
s.find("abc"); : 찾으면 시작 인덱스 반환, 못찾으면 string::npos반환

뒤에서부터 찾기
s.rfind("abc");

부분 문자열 추출
s.substr(pos, len); : pos부터 len개만큼 잘라서 반환

인덱스 접근
s[i] || s.at(i);

문자열 길이
s.length();

비어있는지
s.empty();

메모리 예약
s.reserve(n);

숫자 변환
stoi(s);
stoll(s);
stod(s);

숫자->문자변환
to_string(num);

### Stack

--stack--
st.pop();
st.push(value);
st.top();
st.empty();
st.size();

### Queue

--queue--
q.push(value);
q.pop();
q.front();
q.back();
q.empty();
q.size();

### Deque

--deque--
dq.push_front(value);
dq.push_back(value);
dq.pop_front();
dq.pop_back();
dq[i] || dq.at(i);
dq.insert(dq.begin() + index, value);
dq.erase(dq.begin() + index);
dq.clear();
dq.size();
dq.empty();

# 트러블 슈팅

## 지뢰

!youtube[wNoHWBkumMo?si=rotr8xbr6yb2rQt1]

- 지뢰에 닿았을때 2초뒤에 지뢰가 폭발하는데 2초안에 다시 닿으면 타이머가 다시 2초를 세기 시작하는 버그

### 해결

```c++
void AMineItem::ActivateItem(AActor* Activator)
{
	// 이미 작동하고 있으면 무시
	if (bHasExploded)
	{
		return;
	}

    ..기존로직..
}
```

- MineItem에 중복을 무시하는 조건문을 넣어서 해결하였다.

!youtube[mfVu901dzNQ?si=ezJEgGS1R0syjlw8]

# 총 정리

## Build.cs

- Build.cs에는 프로젝트에 필수적으로 사용하는 기능들을 넣는다.
- UMG는 HUD를 사용하기 위해 필요하다.

## HUD

- HUDWidgetClass는 어떤 UI를 만들지 결정하는 클래스, 붕어빵 틀
- HUDWidgetInstance는 실제 객체를 담는곳

## 파티클

- SpawnEmitterAtLocation은 어떤세계, 어떤파티클을 어느곳에 어떤 회전값으로 보여줄지 결정, 자주쓰는 함수

## CI/CD

- Cook Content : 리소스를 쿠킹함, 리소스를 쿠킹한다는것은 최적화하는것을 말함
- check, verify, ensure등이 있다. 꼭 F5로 디버거를 같이 들고 실행하자
- 언리얼에서 프로파일링 가능, 메모리 추가하려면 별도의 커맨드 입력, GC는 절대 사용시기를 제어하지말자

## 알고리즘

- 행렬 회전시 모든지점의 꼭짓점을 저장하지말고 한곳만 저장하고 역방향으로 돌리자
- 알고리즘 문제풀때 먼저 완탐, 그리디를 의심해보자
- 오늘 헷갈린거는
  - vector.erase(vector.begin()) 이안에 이터레이터 넣어야 하는것정도 헷갈린듯

## 끝으로

- 자주쓰는 알고리즘 메서드들은 매일 타이핑해서 외우자
- 생각하면서 타이핑을 많이 하자 아직 언리얼 C++코드가 익숙하지 않은느낌이 많이든다.
