# C++

## 알고리즘 문제 풀어보기

알고리즘 문제 풀면서 새로 알게된 내용 정리

### 1.

[내 학점을 구해줘] (https://www.acmicpc.net/problem/10984)

- cout << fixed -> 소수점 고정
- cout.precision(n) -> n번째 자리까지 출력

### 2.

[수 정렬하기5] (https://www.acmicpc.net/problem/15688)

- cin.tie(NULL)
- ios::sync_with_stdio(false)
- 백준에서는 cin, cout 속도를 높이기 위해 이 두 설정을 해주어야한다.
- C입출력과의 싱크를 끊어내는것이라고 한다.

### 3.

- [Generations of Tribbles] (https://www.acmicpc.net/problem/9507)

- 벡터는 배열처럼 첫번째 요소가 포인터가 아님
- 배열은 이름 `arr`을 쓰면 자동으로 첫번째요소를 가리키는 포인터로 변하지만
- 벡터는 클래스(객체)이기에 그런식으로 동작하지 않음

```
void koong (int* vec) {...};

koong(&vec[0]);
koong(vec.data());
koong(arr);
```

- 벡터의 경우

  - &vec[0] : 첫번째 요소 주소 꺼내서 줌
  - vec.data() : C++ 11부터 지원, 내부 배열 시작주소(int\*)를 반환

- [내 풀이](https://github.com/JongKyuHong/NBC_TIL/blob/main/3%EC%A3%BC%EC%B0%A8/code/day10/Q1.cpp)

### 4.

- [반지] (https://www.acmicpc.net/problem/5555)

- 키워드 찾는 문제
- string의 find로 키워드를 바로 찾을 수 있음

```
if (ring.find(keyword) != string::npos) {...};
```

- string::npos

  - 타입 : size_t
  - 의미 : 유효하지 않은 인덱스

- [내 풀이](https://github.com/JongKyuHong/NBC_TIL/blob/main/3%EC%A3%BC%EC%B0%A8/code/day10/Q2.cpp)

#### 문자열 메서드

- str.find(찾을거)
- str.substr(시작위치, 길이)

  - 문자열 복사해 잘라옴

- str += "추가할문자열"
- str.replace(시작위치, 길이, "교체할문자열");
- str.erase(시작위치, 길이)
- to_string(숫자)
- stoi(문자열)
- reverse(str.begin(),str.end()) : 문자열 뒤집기, algorithm헤더 필요

### 5.

- [알파벳 찾기] (https://www.acmicpc.net/problem/10809)

```c++
int index = S[i] - 'a';
```

- S[i]가 소문자일때 문자 - 문자를 통해서 둘의 아스키코드 차이만큼 int로 받을 수 있었다.

- [내 풀이](https://github.com/JongKyuHong/NBC_TIL/blob/main/3%EC%A3%BC%EC%B0%A8/code/day10/Q3.cpp)

### 6.

- [단어의 개수] (https://www.acmicpc.net/problem/1152)

- sstream
- istringstream 사용법
- [내 풀이](https://github.com/JongKyuHong/NBC_TIL/blob/main/3%EC%A3%BC%EC%B0%A8/code/day10/Q4.cpp)

### 7.

- [진법 변환] (https://www.acmicpc.net/problem/2745)

- char형 숫자를 계산할때 '0'을 빼주어야 한다는것을 배웠다.

- [내 풀이](https://github.com/JongKyuHong/NBC_TIL/blob/main/3%EC%A3%BC%EC%B0%A8/code/day10/Q5.cpp)
