# 언리얼

## 구현 과정

어제 하던 과제 좀 더 컨텐츠 늘리기

### 구현 과정

#### JumpOrb 변경

적절한 스태틱메시를 찾아서 JumpOrb를 변경해주었다.
![](https://velog.velcdn.com/images/kyu_/post/2286afa7-7949-4c29-8cf3-c0c86c3e3195/image.png)
- 귀여운 코어 모양
- 이에 맞게 이벤트 그래프도 변경해주었고, 마지막 지점에 들어갈때 발판을 제거하고 Orb로만 구성을 했다.

![](https://velog.velcdn.com/images/kyu_/post/196d96e8-3d68-4dc8-8cd5-fda3620b925c/image.png)

- 오브가 없어지면 다시 건널수가 없으니 오브는 destroy되는것이 아니라 몇초뒤에 다시 생성되는것으로 변경하였다.

![](https://velog.velcdn.com/images/kyu_/post/67f03adc-3317-4c1d-ae67-43d1fa121985/image.png)

- Set Actor Hidden In Game, Set Collision Enabled를 통해서 먹은 당시에는 사라지고 4초뒤에 재생성되게끔 만들었다.

![](https://velog.velcdn.com/images/kyu_/post/237a5fb7-856e-4653-a276-db5200e0522a/image.gif)



#### SavePoint만들기

스태틱메시는 동전같이 생긴게 있길래 이걸로 정했다.

![](https://velog.velcdn.com/images/kyu_/post/8427eb72-7d12-4a21-95f1-40593c985734/image.png)

기존에 killzone을 만들때 bp_gamemode를 만들어 놓은것이 있어서 그것을 활용하고자 하였다.

![](https://velog.velcdn.com/images/kyu_/post/4c8113ed-71e4-43d2-9cf3-786e0897a2a4/image.png)

현재 게임모드의 이벤트 그래프는 이렇게 되어있는데, savepoint를 먹었을때 respawn location에 set만 해주면 되었다.

![](https://velog.velcdn.com/images/kyu_/post/4507e3dc-cb6d-4d8a-bbfd-edfcd5f22d82/image.png)

![](https://velog.velcdn.com/images/kyu_/post/1def19e8-05de-4ecc-84f1-f6eec77023a2/image.gif)

- 세이브포인트를 먹으면 그 자리에서 리스폰된다.

#### 발판 버튼 트리거

![](https://velog.velcdn.com/images/kyu_/post/20a7a1af-9946-4c05-abab-90d671d70eed/image.png)

이렇게 흩어져있는 발판들을 앞의 버튼처럼생긴 발판을 누르면 한데 모이게끔 구현하려고 했다.

![](https://velog.velcdn.com/images/kyu_/post/9a47a6e7-a2ac-4843-aa8b-f52ee83a184c/image.png)

- BP_TogglePlatform (직접 움직이는 발판) 블루프린트에 커스텀 이벤트를 만들고

![](https://velog.velcdn.com/images/kyu_/post/3d702d8d-f6f7-43ae-9d90-8300583bdb9d/image.png)

![](https://velog.velcdn.com/images/kyu_/post/89957d7c-cd4b-4b0f-98a2-c5e7b3de1fd8/image.png)

- BP_TriggerButton(트리거 버튼) 블루프린트에는 BP Toggle Platform을 Array변수로 만들어서 뷰포트에서 하나씩 지정해주었다.

![](https://velog.velcdn.com/images/kyu_/post/eabf3cf0-c333-411f-9a5f-54bfab373c58/image.gif)

- 완성!

#### 게임 종료

![](https://velog.velcdn.com/images/kyu_/post/1cc52ae7-a4b3-4d20-96ab-8ebef182bf0e/image.png)

- ~~미니 프로젝트 우려먹기~~

![](https://velog.velcdn.com/images/kyu_/post/1141d742-55d3-4eb4-a370-a0d5ec81fac1/image.gif)

- 오브를 먹으면 Game Clear!가 출력되고 3초후에 꺼진다.

#### 약간의 조작감 개선

![](https://velog.velcdn.com/images/kyu_/post/f3832c9d-8e5a-4c07-af58-51098b232eea/image.gif)

- 점프할때 움직이는 발판 속도에 영향을 받는듯한 느낌이 들었다.

![](https://velog.velcdn.com/images/kyu_/post/597bf079-8bcc-4776-99f8-e4ff7176beea/image.png)

- Impart Base
  - Impart Base는 캐릭터가 서있는 Base의 속도를 캐릭터에게 얼마나 넘겨줄지에대한 속성이라고 한다.
  - 이를 전체 해제해줌으로써 조작감 개선을 이뤘다.
  
![](https://velog.velcdn.com/images/kyu_/post/248568a5-ba11-4fac-8714-cfa5c9427a5b/image.gif)




# C++

## 우선순위 큐

- 내부적으로 힙 사용
- 삽입, 삭제 O(log N), top()은 O(1)
- 가장 큰/작은 원소를 추출해야하는 문제에서 효율적

- 특정 기준으로 우선순위를 부여하고 싶다면 사용자 정의 함수를 넣어줄 수 있다.

```
#include <iostream>
#include <queue>
#include <vector>

using namespace std;

struct Student
{
	string name;
	int score;
};

struct Compare
{
	bool operator()(const Student& a, const Student& b)
	{
		return a.score < b.score;
	}
};

int main()
{
	priority_queue<Student, vector<Student>, Compare> pq;

	pq.push({ "tom", 95 });
	pq.push({ "goong", 80 });
	pq.push({ "rtan", 99 });

	return 0;
}
```

### 급 궁금한거

#### 1번 질문

지금 예시에서 우선순위큐의 첫번째인자, 두번째 인자인 vector의 타입으로 구조체가 들어갔는데 이런식으로 사용할 수 있는건지?

- C++ STL컨테이너는 어떤 타입이든 지정 가능하다.
- vector<Student> 제네릭이 아니고 C++에서는 템플릿 매개변수, 타입 매개변수라고 부른다.
- 우리가 기억할건 템플릿에 구조체 활용이 가능하다.

#### 2번 질문

```
struct Compare {
    bool operator()(const Student &a, const Student &b) {
        return a.score < b.score;
    }
};

...
.....
priority_queue<Student, vector<Student>, Compare> pq;
```

세번째 인자에 구조체가 들어있고 operator()(..) 이라는 특이한 구조인데 어떤 의미인지?

- C++에서 연산자 오버로딩은 정해진 이름이 있다.
  - operator+, operator==, operator[], operator()
  - 여기서 operator()는 함수호출 연산자() 오버로딩이다.

### Q.최소 힙 구현시 greater<int>를 넣지 않고 해결하는 방법을 한 번 생각한 뒤 코드로 구현

```
#include <iostream>
#include <queue>

using namespace std;

struct CompareStruct {
	bool operator()(int& a, int& b)
	{
		return a > b;
	}
};


int main()
{
	priority_queue<int, vector<int>, CompareStruct> pq;

	pq.push(1);
	pq.push(2);
	pq.push(3);
	pq.push(4);
	pq.push(5);

	cout << "최소힙으로 정렬 되었는지 : " << pq.top() << endl;

	return 0;
}
```

- 다른 언어들 처럼 음수넣고 나중에 뺄때 -1곱해주는 방식도 있다.

## 순열

- next_permutation : 주어진 범위를 사전순으로 다음 순열로 바꿔줌
- prev_permutation : 주어진 범위를 사전수으로 이전 순열로 바꿔줌
- 원본 데이터는 정렬상태여야 한다.
- 주어진 순열의 모든 순열 생성, 사전순으로 특정 순열 찾기와 같은 미션을 받을때 유용하다.

```
vector<int> v = {1, 2, 3};

do{
	for (int x : v) cout << x << " ";
	cout << endl;
} while (next_permutation(v.begin(), v.end()));

// 모든 순열을 구할 수 있음
// do while문을 통해서 다음 순열이 존재한다면 출력
```

Q. 거꾸로 출력

```
// vector<int> v = { 3, 2, 1 };
vector<int> v = {1, 2, 3};

sort(v.begin(), v.end(), greater<int>());

do {
	for (int x : v) cout << x << " ";
	cout << endl;
} while (prev_permutation(v.begin(), v.end()));

return 0;
```

### K번째 값 찾기

- nth_element사용
- pivot으로 n번째 작은 원소 배열의 n번째 위치로 보낸 뒤, n번째 보다 작은원소들은 왼쪽, 큰것들은 오른쪽으로 반정렬 해줌 약간 퀵소트랑 비슷하게
- 평균 시간 복잡도가 O(n)으로 빠름
- nth_element(first, nth, end);

```
vector<int> v = {9, 1, 4, 7, 2, 6};

nth_element(v.begin(), v.begin()+2, v.end());

cout << "3번째로 작은 원소 : " << v[2] << endl;
```

Q. 배열크기가 홀수 일때 중간 값을 찾는것을 nth_element코드 하나만으로 구현

```
nth_element(v.begin(), v.begin() + v.size()/2, v.end());
```

Q1. 배열 {15, 3, 9, 8, 5, 2, 10, 7, 6}에서 짝수만 고려하여 3번째로 큰 수를 출력하세요. (10분)

- [내 풀이](https://github.com/JongKyuHong/NBC_TIL/blob/main/2%EC%A3%BC%EC%B0%A8/code/day5Q1.cpp)

Q2. 배열 {1, 2, 3, 4}에서 길이가 3인 순열을 모두 출력하세요. 단, 순열의 첫 번째 원소는 항상 1이어야 합니다.(10분)

- [내 풀이](https://github.com/JongKyuHong/NBC_TIL/blob/main/2%EC%A3%BC%EC%B0%A8/code/day5Q2.cpp)

Q3. 배열 {4, 1, 7, 3, 8, 5}를 5 이상의 값만 내림차순으로 출력하세요. (10분)

- [내 풀이](https://github.com/JongKyuHong/NBC_TIL/blob/main/2%EC%A3%BC%EC%B0%A8/code/day5Q3.cpp)

## STL 알고리즘 2

### 연속 구간 합 구하기

- partial_sum : numeric헤더에 있으며 누적합을 계산해준다.
  - partial_sum(v.begin(), v.end(), result.begin())
    - 처음부터 i번째 까지의 합을 i번째 인덱스에 넣음

### 유니크 값 구하기

- unique는 연속된 중복원소를 제거하여 끝위치 iterator를 반환
- 정렬이 필수이다.
- 비중복되는 원소들을 앞으로 당기면서 중복을 제거하고 끝위치 이터레이터를 반환하는데 이를 원본 배열의 erase를 통해 깔끔하게 지워낸다.

### 총합 구하기

- accumulate는 총합을 구하는데 사용
- accumulate(begin() , end(), 시작값)
- 4번째 인자로 사용자 정의 함수를 넣을 수 있다.
