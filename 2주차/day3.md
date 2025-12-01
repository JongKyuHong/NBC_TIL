# 언리얼

## DAY 3 블루프린트를 이용한 비주얼 스크립팅

### 블루프린트 변수

- Boolean : 참/거짓
- Byte : 정수, 가장 크기가 작음
- Integer : 정수, 일반적인 크기(-21억~21억)
- Float : 실수, 정밀도가 우월하다(double-precision)
- Name : 문자열, 엔진 내부에서 사용, 불변인 경우
- String : 문자열, 일반적인 문자열
- Text : 문자열, 퀘스트 설명같이 다국어 변환이 필요한 경우

### 1번 과제

![](https://velog.velcdn.com/images/kyu_/post/d2a57c41-693b-4b9d-8fef-58db488b7246/image.png)

### 2번 과제

![](https://velog.velcdn.com/images/kyu_/post/b5e1da28-628f-4a0b-bb0e-3f1f0e05c2cb/image.png)

![](https://velog.velcdn.com/images/kyu_/post/999480d3-5024-4f93-abc4-05a5f099635e/image.png)

- 해당 블루프린트를 보면 중복되는 로직이 있다 이를 함수로 빼서 재사용할것이다.

![](https://velog.velcdn.com/images/kyu_/post/d50c4bcc-cbe6-4d08-a224-753dfcc66e5b/image.png)

![](https://velog.velcdn.com/images/kyu_/post/81ab57a6-ae74-4d5e-8990-05d716aded4e/image.png)

- 개선된 부분

- 쿨다운은 그냥 increment만 -1로 주면 됨

### 3번 과제

![](https://velog.velcdn.com/images/kyu_/post/5a38e976-13cf-4a92-b44f-24eeb556e1d6/image.png)

![](https://velog.velcdn.com/images/kyu_/post/4447f0dc-d240-4fef-abb9-f2b277b22c3c/image.png)

![](https://velog.velcdn.com/images/kyu_/post/8b4932d8-bbfb-4fb7-8641-3a128af8ff3e/image.png)

![](https://velog.velcdn.com/images/kyu_/post/4a368638-e732-4386-9be6-462e21857644/image.png)

- struct인스턴스에 접근할때 get copy로 접근해서 멤버 변수들에 접근가능
- set할때는 set array elem을 통해 target 구조체 배열과 인덱스를 부여해서 원하는 구조체배열 인스턴스에 값을 갱신해줄수있음
- format text도 잊지말자

## 복습

### 회전문 만들기

- 저번시간에 진행했던 회전문을 안보고 다시 만들어보았다.
- 캐릭터가 다가가면 돌아가는 문, 문은 한번 돌아가면 캐릭터가 멀어질때까지 다시 돌지 않음

#### 설계

1. beginoverlap에서 캐릭터를 cast
2. 정상적으로 cast가 되면 좌표를 계산함 (우리 기준에서는 z축 -90도)
   2-1. startRotation의 초기 rotation값은 get actor rotation을 받아오면 됐었던것 같음
3. 계산한 좌표를 timeline에 넣음
4. timeline과 lerp를 이용해서 좌표값을 천천히 돌리게 만듬
5. 이 return 들을 set relative rotation에다가 연결
6. set relative rotation의 주체는 root로 연결하면 될듯
7. StartRotation, TargetRotation set해줌
8. 재발 방지를 위해 do once를 지정해 주어야 하는데 이는 cast성공시로 잡으면 될듯함
9. do once의 reset값은 endoverlap에 캐릭터가 cast되었을때로 하면 됨

![](https://velog.velcdn.com/images/kyu_/post/c06f022f-76be-415d-8c2a-112167d36dce/image.png)

#### 결과

![](https://velog.velcdn.com/images/kyu_/post/580ff99d-6bf4-4054-8577-25d3a73697bb/image.gif)

- 의도한대로 잘 동작한다!

# C++

## 베이직 C++ 기초반

### 포인터 사용해보기

```
#include <iostream>

using namespace std;

struct Player
{
	string Name;
	int Hp;
};

void DamageByValue(Player p, int Damage)
{
	p.Hp -= Damage;
	cout << "[DamageByValue] 내부 p.Hp = " << p.Hp << endl;
}

void DamageByPointer(Player* p, int Damage)
{
	p->Hp = p->Hp - Damage;
	cout << "[DamageByPointer] 내부 p.Hp = " << p->Hp << endl;
}

int main()
{
	Player SpiderMan;
	SpiderMan.Name = "톰 홀랜드";
	SpiderMan.Hp = 100;

	cout << "시작 SpiderMan.HP = " << SpiderMan.Hp << endl << endl;

	DamageByValue(SpiderMan, 30);

	cout << "값 전달 후 SpiderMan.Hp = " << SpiderMan.Hp << " (그대로) " << endl << endl;

	Player* ptrSpiderMan = &SpiderMan;
	DamageByPointer(ptrSpiderMan, 40);

	cout << "포인터 전달 후 SpiderMan.Hp = " << SpiderMan.Hp << " -40됨 " << endl << endl;

	return 0;

}

```

- 정답코드를 보면 DamageByPointer내부에서 p != nullptr로 처리하는거 기억하기
- p->HP -= Damage; 로 사용가능

### cin, printf()연습

```
#include <iostream>
#include <string>

using namespace std;

int main()
{
	cout << "당신의 이름은 : ";
	string Name;
	cin >> Name;

	cout << "당신의 나이는 : ";
	int age;
	cin >> age;

	printf("\n===================\n\n");

	printf("이름 : %s \n", Name.c_str());
	printf("나이 : %d", age);
}
```

- string출력하려면 c_str()로 C++스타일의 문자열을 C스타일로 변경해서 출력

## 스탠다드반 C++

### 6강 문제 풀이

#### 문제1. 서로 다른 부분 문자열의 개수

- [내 풀이](https://github.com/JongKyuHong/NBC_TIL/blob/main/2%EC%A3%BC%EC%B0%A8/code/%EC%84%9C%EB%A1%9C%EB%8B%A4%EB%A5%B8%EB%B6%80%EB%B6%84%EB%AC%B8%EC%9E%90%EC%97%B4%EC%9D%98%EA%B0%9C%EC%88%98.cpp)

- S의길이가 1000이하이기 때문에 이중for문을 사용해도 되겠다고 생각했습니다.
- 서로 다른것의 개수니까 Set을써서 중복을 제거하는게 빠를거라고 생각했습니다.

#### 문제2. 숫자 카드

- [내 풀이]
- 값이 존재하는지 확인하는 부분이 있기때문에 map을 사용하면 효율적일것이라고 생각했습니다.
- 값의 존재만 확인하면 되기때문에 unordered_map을 사용하는것이 훨씬 효율적이라는것을 배웠습니다.
- 지금처럼 result를 이어붙여서 출력하기 보다는 바로 출력하는게 제일 나을것 같습니다.

- [개선해본 풀이]

- 수정 전 속도
  ![](https://velog.velcdn.com/images/kyu_/post/93e4d32c-30ff-40e7-8a7d-a0ffcc01fd2b/image.png)
- 수정 후 속도
  ![](https://velog.velcdn.com/images/kyu_/post/c47f1aca-58ee-4b70-94ad-4d20860372ab/image.png)
- unordered*set사용 속도
  ![](https://velog.velcdn.com/images/kyu*/post/229669e3-34ac-44f6-b6de-ce615f023264/image.png)

- 정렬이 필요없는 문제에서는 unordered 자료구조를 사용해서 더 효율적으로 문제를 풀어야 겠다는것을 배웠습니다.

#### 문제3. 대칭 차집합

- [내 풀이]
- 차집합이기 때문에 조회를 해야하므로 map이 효율적일것이라고 생각
- set으로 합집합에서 중복을 제거
- 이 문제도 unordered 자료구조를 사용하면 더 효율적일것이라고 생각했다.

- [내 풀이 개선]
- 개선 전
  ![](https://velog.velcdn.com/images/kyu_/post/a4b87f5e-1df0-4c6e-873c-0ac25984f172/image.png)
- 개선 후
  ![](https://velog.velcdn.com/images/kyu_/post/8ab70a10-9713-42c3-a53c-aef728524cb3/image.png)

### 기존 배운 내용 복습

- set, map, 문자열 위주로 백준문제들 풀어보자

- 문자열
  - https://www.acmicpc.net/problem/2941
