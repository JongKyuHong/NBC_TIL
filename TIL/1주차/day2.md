# 언리얼

### 공부한 개념들

- Add Movement Input노드

  - 입력값을 캐릭터의 Movement Component에 넘겨주는 노드
  - world direction으로 어디방향으로 움직이고 싶은지를 입력받음, 보통 forward vector나 right vector같은 방향 벡터를 입력

- Get Control Rotation노드

  - 플레이어 컨트롤러가 지금 어디를 바라보고 있는지를 Rotator로 가져오는 노드

- Add Controller Yaw/Pitch/Roll Input노드

  - 컨트롤러의 ControlRotation(시점 방향)에 회전 입력을 더해주는 노드
  - Yaw : 좌우 회전
  - Pitch : 상하 회전
  - Roll : 옆으로 기울기(Gta 비행기)
  - Val 값으로 이번 프레임에 얼마나 회전 시킬지를 결정
    - 우리의 경우 Look action input에서 받아서 사용

- Scalar

  - 한개의 숫자 값
  - float 1.0, -0.05, Speed = 600.0
  - 얼마만큼 회전/이동/스케일할지 등 양을 표현할때 사용

- Vector

  - 방향 + 크기를 가진 3차원 값
  - 언리얼에서는 주로 이렇게 사용
    - 위치 : Location(월드에서 어디 있는지)
    - 방향 : Direction -> 정규화 벡터(Forward Vector)
    - 힘, 가속도 등

- Velocity(속도)
  - 물리/이동에서의 '단위 시간당 위치 변화' = 속도 벡터
  - Get Velocity를 하면 FVector값을 반환
    - Vector Length노드를 만나서 속도를 반환할수도있고(스칼라)
    - Get Safe Normal로 정규화하여 이동 방향 벡터를 얻을 수도 있다. (어디로 이동하는지 방향)
- Event Blueprint Update Animation노드

  - AnimBP의 Event Graph에서 매 프레임 호출되는 이벤트
  - Event tick과 비슷하게 동작한다고 보면 됨

- Try get pawn owner노드

  - 이 AnimBP가 어떤 Pawn(캐릭터)에 붙어서 동작 중인지 가져오는 노드
  - 리턴타입은 Pawn(참조)

- Get Movement Component 노드

  - Actor/Pawn/Character에 붙어 있는 Movement Component(이동 담당 컴포넌트)를 가져오는 노드
  - 이 노드로 부터 Velocity, IsFalling, Max Walk Speed 같은 이동 관련 정보를 얻을 수 있다.

- Do Once노드

  - 실행 신호가 들어오면 딱 한 번만 통과시켜줌, Reset을 실행 한 뒤 다시 실행가능
  - Start Closed : Do Once노드가 처음부터 닫힌 상태로 시작할지를 결정하는 옵션

- Timeline노드

  - 시간에 흐름에 따라 변화시켜주는 노드
  - 입력
    - Play : 멈춰있는 그 시점부터 재생
    - Play from Start : 무조건 0초부터 다시 실행
    - Stop : 타임라인을 즉시 멈춤
    - Reverse : 현재 위치에서 시간을 거꾸로 돌림
    - Reverse from End : 무조건 끝(End)에서부터 거꾸로 재생
  - 출력
    - Update : 타임라인이 재생되는 동안 매 프레임 신호를 보냄
    - Finished : 타임라인이 다 끝났을때 딱 한번 신호를 보냄
    - Track : 개발자가 그래프로 찍어둔 0~1사이의 변하는 값이 나옴
    - Direction : 현재 타임라인이 어느 방향으로 재생되고 있는지
      - Forward : Play, Play from Start로 정방향 재생 중
      - Backward : Reverse, Reverse from End로 역방향 재생 중

- Lerp(Linear Interpolation, 선형 보간)
  - A값과 B값 사이의 중간값을 찾아줌
  - A 시작점, B 목표점
  - Alpha : 비율, 0.0 -> A값, 1.0 -> B값, 0.5 -> 딱 중간값을 뱉음
  - 보통 Timeline의 track값을 꽂아서 타임라인이 재생되는 동안 결과값이 A에서 B로 부드럽게 변함

### 구현하며 배운 것

#### 1\. 속도 측정 방식

![](https://velog.velcdn.com/images/kyu_/post/d4315d7a-50b2-47b3-9369-ade353b98e9f/image.png)

![](https://velog.velcdn.com/images/kyu_/post/e1dc2639-17ae-452e-ba4c-ca22ee895e41/image.png)

get velocity -> vector length를 통해서 속도를 구하는 두방식의 차이점이 궁금했다.

첫번째 방식은 Z값의 속도도 받기 때문에 3D속도가 되고, 점프나 낙하로 Z속도가 커지면 Speed값도 같이 커지게 된다. 그러므로 위로 점프하거나 떨어지는 속도도 달리기 속도에 섞여 들어 가므로 캐릭터 이동의 Speed 변수는 X/Y만 사용하는 식으로 많이 쓰인다.

#### 2\. Automatic Rule Based on Sequence Player in State

State Machine에서 다른 state로 넘어갈때 'Automatic Rule Based on Sequence Player in State' 이런속성이 있다. 이는 현재 상태의 애니메이션이 끝까지 재생되면 자동으로 다음 상태로 넘어가라는 뜻

#### 3\. 다가갈때마다 부드럽게 돌아가는 회전문 만들기

##### 수많은 시도들

- 초기 블루프린트
  ![](https://velog.velcdn.com/images/kyu_/post/6439a192-09b7-4ad7-b62c-2eb2860d9d35/image.png)

- Timeline앞으로 isClose를 식별하는 브랜치를 옮김
  ![](https://velog.velcdn.com/images/kyu_/post/8107db7b-3fed-4206-a96b-01d22e1ce9d1/image.png)

- isClose대신에 Do Once노드 사용
  ![](https://velog.velcdn.com/images/kyu_/post/f40a94b7-98a2-4443-b1fb-166d9d9b9a03/image.png)

- Start, TargetRotation을 Timeline앞에 배치
  ![](https://velog.velcdn.com/images/kyu_/post/c2551ad6-a63a-40be-a8fa-f6900db98050/image.png)

- Do Once의 Reset과 end overlap연결
  ![](https://velog.velcdn.com/images/kyu_/post/2eaeb74c-291b-4c33-8540-2aca09fe3515/image.png)

  - 문이 첫번째 접근할때는 Timeline을 타서 부드럽게 열리는데 두번째이후로는 바로바로 z축 -90도씩 움직여버림

- Timeline의 Play from Start
  ![](https://velog.velcdn.com/images/kyu_/post/0c7effc8-4692-40e5-bd64-03caaba57c52/image.png)

  - Timeline의 Play가아니라 Play for Start에 연결하면 Timeline이 매 실행마다 다시 0초부터 실행됨

- ★완성품★
  ![](https://velog.velcdn.com/images/kyu_/post/25142ed9-f8ae-4872-ae62-fd6f539812c4/image.gif)

#### 4\. Run 애니메이션

- 달리는 애니메이션이 고정되는 문제
  ![](https://velog.velcdn.com/images/kyu_/post/6a104908-1cb6-4ad2-9454-eee71f8d3ba3/image.gif)

![](https://velog.velcdn.com/images/kyu_/post/8dcc475a-b64c-48e2-8e65-53984234a864/image.png)
loop 설정해서 해결

# C++

## 실습문제 풀기

### 문제 설명

정수 `N`(1 ≤ N ≤ 1000)을 입력받고, 이어서 `N`개의 정수를 입력받는다. 이후 프로그램은 다음을 계산해야 한다.

1. 최댓값, 최솟값
2. 평균값 (가능하다면, 소수점 첫째 자리까지 (못해도 상관 X))
3. 가장 자주 등장한 수 (최빈값)
   - 여러 개면 가장 작은 값을 출력한다.
4. 짝수의 개수와 홀수의 개수
5. 모든 값의 절댓값 평균 (가능하다면, 소수점 첫째 자리까지 (못해도 상관 X))

단, **STL 컨테이너/알고리즘을 일절 사용하지 말 것**. (`vector`, `algorithm`, `map`, `sort`, `set` 등 금지)

### 입출력 형식

### 예시 입력

```
10
3 -5 3 7 2 0 3 12 8 -1
```

### 예시 출력

```
min: -5
max: 12
avg: 3.2
mode: 3
even: 4
odd: 6
abs_avg: 4.4
```

- 내 풀이

```
#include <iostream>
#include <string>

using namespace std;

int main() {
	int N;
	cin >> N;
	int arr[1001];
	for (int i = 0; i < N; i++)
	{
		int inp;
		cin >> inp;
		arr[i] = inp;
	}

	int maxV = 0;
	int minV = 2 ^ 32;
	double sumV = 0.0;
	int odd = 0;
	int even = 0;
	int arrMaxV = 0;
	int arrMaxIdx = -1;
	double absSumV = 0.0;

	for (int i = 0; i < N; i++)
	{
		int inp = arr[i];

		if (maxV < inp)
		{
			maxV = inp;
		}
		else if (minV > inp)
		{
			minV = inp;
		}

		sumV += inp;

		if (inp % 2)
		{
			odd++;
		}
		else
		{
			even++;
		}

		if (inp < 0)
		{
			absSumV -= inp;
		}
		else
		{
			absSumV += inp;
		}
	}

	int mVCnt = 0;
	int mV = 0;

	for (int i = 0; i < N; i++) {
		int inp = arr[i];
		int cnt = 0;
		for (int j = 0; j < N; j++) {
			if (inp == arr[j])
			{
				cnt++;
			}
		}
		if (mVCnt < cnt)
		{
			mVCnt = cnt;
			mV = inp;
		}
	}

	cout << fixed;
	cout.precision(1);
	cout << "min: " << minV << endl;
	cout << "max: " << maxV << endl;
	cout << "avg: " << sumV / N << endl;
	cout << "mode: " << mV << endl;
	cout << "even: " << even << endl;
	cout << "odd: " << odd << endl;
	cout << "abs_avg: " << absSumV / N << endl;


	return 0;
}
```

- cin은 입력이 숫자일때 반복문으로 돌리면 알아서 공백 건너뛰고 입력받는다.
- 최빈값 구하는법을 몰라서 이중for문으로 만듬
- std::cout << std::fixed; std::cout.precision(n);으로 소수점 n까지 출력가능하다는것을 알게되었음
- int/int는 무조건 정수만 뱉는데 까먹고 sumV와 absSumV를 int로 선언함 --> double로 수정

## 개념들

### STL

C++에 내장된 템플릿 기반의 라이브러리

- 컨테이너 : 데이터를 저장, 관리하는 구조체들의 집합
- 반복자 : 컨테이너 내 데이터를 순회할 수 있도록 도와주는 일종의 '포인터'
- 알고리즘 : 정렬, 탐색, 삽입, 삭제 등과 같은 로직을 매우 효율적이고 제네릭하게 제공

### 컨테이너

- vector : 동적 배열

```
#include <iostream>
#include <vector>


int main() {
	std::vector<int> vec;

	vec.push_back(5);
	vec.push_back(6);
	vec.push_back(7);

	vec.pop_back();

	std::cout << "처음: " << vec.front() << std::endl;
	std::cout << "마지막: " << vec.back() << std::endl;

	vec.insert(vec.begin() + 1, 15);
	for (int elem : vec)
	{
		std::cout << elem << " ";
	}

	std::cout << std::endl;

	vec.erase(vec.begin() + 1);
	for (int elem : vec)
	{
		std::cout << elem << " ";
	}
	std::cout << std::endl;

	return 0;
}
```

- List : 이중 링크드 리스트

```
#include <iostream>
#include <list>

int main() {
	std::list<int> lst;

	lst.push_back(30);
	lst.push_back(31);
	lst.push_back(32);

	for (int elem : lst)
	{
		std::cout << elem << " ";
	}
	std::cout << std::endl;

	auto it = lst.begin();
	++it;
	lst.insert(it, 15);

	for (int elem : lst)
	{
		std::cout << elem << " ";
	}
	std::cout << std::endl;

	return 0;
}
```

- Deque(데크)

```
#include <iostream>
#include <deque>

int main() {
	std::deque<int> dq;

	dq.push_back(1);
	dq.push_front(2);
	dq.push_back(3);

	std::cout << "맨 앞: " << dq.front() << std::endl;
	std::cout << "맨 뒤: " << dq.back() << std::endl;

	dq.insert(dq.begin() + 1, 4);

	std::cout << "두 번째 원소: " << dq[1] << std::endl;

	dq.erase(dq.begin() + 1);

	std::cout << "두 번째 원소: " << dq.at(1) << std::endl;

	dq.pop_front(); // 맨 앞 원소제거
	dq.pop_back(); // 맨 뒤 원소제거

	return 0;
}

```
