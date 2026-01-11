# 복습 세션

## 주요 STL

### Vector

- 연속된 메모리 공간에 데이터 저장
- 끝에 추가/삭제빠름 O(1), 중간 삽입/삭제 느림 O(N)
  - vec.insert(v.begin() + index, value) : 중간 삽입

### List

- 양방향 연결 리스트
- 불연속 메모리, 노드들이 포인터로 연결, 인덱스 접근 불필요할때
- 어디서든 삽입/삭제 빠름 O(1) 대신 위치를 알아야 함

```c++
#include <iostream>
#include <list>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(NULL);

    // 1. 초기화 방법
    list<int> l; // 빈 리스트
    list<int> l2(5); // 크기5, 기본값 0
    list<int> l3(5, 10); // 크기5, 기본값 10
    list<int> l4 = { 1,2,3,4 }; // 초기화 리스트

    // 2. 데이터 추가
    l.push_back(10);
    l.push_back(20);
    l.push_back(30);
    l.push_front(5); // 앞쪽에 추가

    // 3. 데이터 접근
    cout << "front: " << l.front() << endl;  // 5
    cout << "back: " << l.back() << endl;    // 30

    // 4. 데이터 삭제
    l.pop_back();
    l.pop_front();

    // 5. iterator로 순회
    cout << "리스트 출력: ";
    for (auto it = l.begin(); it != l.end(); it++) {
        cout << *it << " ";
    }
    cout << endl;

    // for-each
    for (int x : l) {
        cout << x << " ";
    }

    // 6. insert로 중간 삽입
    // 인덱스 1에 15를 추가하는 상황
    auto it = l.begin();
    it++;
    l.insert(it, 15); // 1번째 인덱스에 15추가


    // 7. erase로 중간 삭제
    // 인덱스 2값 삭제
    it = l.begin();
    it++;
    it++;
    l.erase(it);

    // 8. 상태확인
    cout << "size: " << l.size() << endl;      // 2
    cout << "empty: " << l.empty() << endl;    // false (0)

    // 9. 전체 삭제
    l.clear();
    return 0;
}

```

#### 문제 상황 (인덱스 접근)

```c++
// 6. insert로 중간 삽입
l.insert(l.begin() + 1, 15); // 1번째 인덱스에 15추가


// 7. erase로 중간 삭제
l.erase(l.begin() + 1);
```

- List에서는 인덱스 접근이 없고 Iterator도 포인터로 한칸씩 전진하면서 확인을 하기 때문에 위 코드처럼 사용을 못함

```c++
// 6. insert로 중간 삽입
// 인덱스 1에 15를 추가하는 상황
auto it = l.begin();
it++;
l.insert(it, 15); // 1번째 인덱스에 15추가


// 7. erase로 중간 삭제
// 인덱스 2값 삭제
it = l.begin();
it++;
it++;
l.erase(it);

```

- 이런식으로 접근해야 한다.

### Deque

- 양방향 큐
- 양쪽 끝에서 삽입/삭제 빠름
- vector + list의 장점 결합

### 나머지

- Stack
- Queue
- Set
- Map
- unordered_map
- unordered_set

### 우선순위 큐

```c++
#include <iostream>
#include <queue>
#include <vector>

using namespace std;

struct Student{
    string name;
    int score;
}

struct Compare{
    bool operator()(const Student& a, const Student& b){
        return a.score < b.score;
    }
}

int main(){
    priority_queue<Student, vector<Student>, Compare> pq;

    pq.push({"tom", 95});
    pq.push({"goong", 80});
    pq.push({"rtan", 99});

    return 0;
}
```

- 우선순위큐는 비교조건으로 구조체만 받을 수 있다. 함수는 불가
  - sort는 둘다 가능
- a.score < b.score 즉 b가 크면 true이므로 b가 더 우선순위가 높다. 내림차순 기준 정렬이 된다.

### 순열

```
vector<int> v = {1, 2, 3};

do{
	for (int x : v) cout << x << " ";
	cout << endl;
} while (next_permutation(v.begin(), v.end()));
```

- next_permutation : 주어진 범위를 사전순으로 다음 순열로 바꿔줌
- prev_permutation : 주어진 범위를 사전수으로 이전 순열로 바꿔줌

### K번째 값 찾기

- nth_element : 전체를 정렬하지 않고 n번째 원소만 정확한 위치에 놓는 유용한 함수, algorithm헤더 필요

```c++
vector<int> v = {9, 1, 4, 7, 2, 6};

nth_element(v.begin(), v.begin()+2, v.end());

cout << "3번째로 작은 원소 : " << v[2] << endl;
```

Q1. 배열 {15, 3, 9, 8, 5, 2, 10, 7, 6}에서 짝수만 고려하여 3번째로 큰 수를 출력하세요. (10분)

```c++
vector<int> vec = { 15, 3, 9, 8, 5, 2, 10, 7, 6 };

vector<int> vec2;

for (int v : vec) {
    if (v % 2 == 0) {
        vec2.push_back(v);
    }
}

nth_element(vec2.begin(), vec2.begin() + vec2.size() - 3, vec2.end());

cout << vec2[vec2.size() - 3];
```

Q2. 배열 {1, 2, 3, 4}에서 길이가 3인 순열을 모두 출력하세요. 단, 순열의 첫 번째 원소는 항상 1이어야 합니다.

```c++
 vector<int> vec = { 1, 2, 3, 4 };

 vector<int> vec2 = { 2, 3, 4 };

 do {
     cout << "1" << vec2[0] << vec2[1] << "\n";
 } while (next_permutation(vec2.begin(), vec2.end()));
```

Q3. 배열 {4, 1, 7, 3, 8, 5}를 5 이상의 값만 내림차순으로 출력하세요.

```c++
vector<int> vec = { 4, 1, 7, 3, 8, 5 };
priority_queue<int> pq;

for (int v : vec) {
    if (v >= 5) {
        pq.push(v);
    }
}

while (!pq.empty()) {
    cout << pq.top() << "\n";
    pq.pop();
}
```

### 누적합 구하기

- partialsum : 누적합 계산, numeric헤더

```c++
#include <numeric>
#include <vector>
using namespace std;

vector<int> v = {1, 2, 3, 4};
vector<int> result(4);

partial_sum(v.begin(), v.end(), result.begin());
// result = {1, 3, 6, 10}
```

### 유니크 값 구하기

- unique : 연속된 중복원소를 제거하여 끝 위치 iterator반환
- 정렬 필수
- erase와 함께 깔끔하게 제어가능

```
vector<int> v = {1, 1, 2, 2, 3, 3, 3, 4};

// 1. 정렬
sort(v.begin(), v.end());

// 2. unique로 중복 제거
auto it = unique(v.begin(), v.end());

// 3. erase로 실제 제거
v.erase(it, v.end());
```

### 총합 구하기

- accumulate는 총합 구하는데 사용
- accumulate(begin(), end(), 시작값);
- 4번째 인자로 사용자 정의 함수 넣을 수 있음

```c++
int product = accumulate(v.begin(), v.end(), 1,[](int a, int b) { return a * b; });
```

## 자유주제

### 리터럴과 상수값의 차이

- 리터럴은 코드상에 10이라고 적어놓으면 이게 그냥 리터럴
- 상수는 값이 변하지 않는 공간 const int maxScore = 100에서 maxScore가 상수, 100은 리터럴

### size_t

- 가장 큰 양수 정수, 부호가 없는 정수를 나타낼때 사용
- 배열의 크기나 메모리 주소계산처럼 절대 음수가 될 수 없고 아주 큰수도 있는 숫자에 사용

### 생성자 소멸자

- 생성자는 부모 -> 자식 순으로 호출
- 소멸자는 자식 -> 부모 순으로 호출

## 알고리즘

### 1.

cout << fixed -> 소수점 고정
cout.precision(n) -> n번째까지 출력

### 2.

string의 find
if (ring.find(keyword) != string::npos) {};
해당하는값이 없으면 string::npos를 반환한다.

- str.find(찾을거)
- str.substr(시작위치, 길이)
  - 문자열 복사
- str += "추가할 문자열"
- str.replace(시작위치, 길이, "교체할문자열");
- str.erase(시작위치, 길이)
- reverse(str.begin(), str.end()) : 알고리즘 헤더 필요, 문자열 뒤집기
- str = string(1, char) + char : 이렇게 해야 문자를 문자열로 변환가능

### 3.

알파벳 찾기
int index = S[i] - 'a';
S[i]가 소문자일때 -를 통해서 둘의 아스키코드차이 만큼을 int로 저장가능하다.


