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
