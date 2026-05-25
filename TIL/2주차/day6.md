# C++

C계열 언어의 경우 주소로 접근해서 처리를 하기 때문에 더 빠르다. 어떤 형식 어떤 크기 어떤 주소 이름까지 구닥다리 느낌으로 넣어주니까 속도가 더 빠르다

## 게임 개발자를 위한 C++ 문법

- CTRL + F7 컴파일
- F5 실행
- #include <iostream>

  - 표준 입출력 라이브러리
  - #include를 통해 해당 헤더 파일에 정의도니 기능을 사용할 수 있다

- float와 double은 정밀도 차이고, C++은 기본적으로 double로 실수를 처리하려고 해서 실수뒤에 f를 붙여주면 float로 처리

### 다이아몬드 모양 별 찍기

```
#include <iostream>
#include <algorithm>
#include <vector>
#include <queue>
#include <numeric>

using namespace std;

void solution(int N)
{
	for (int i = 0; i < N; i++)
	{
		for (int j = 0; j < (N-1) - i; j++)
		{
			cout << " ";
		}
		for (int j = 0; j < i*2+1; j++)
		{
			cout << "*";
		}
		for (int j = 0; j < (N - 1) - i; j++)
		{
			cout << " ";
		}
		cout << endl;
	}

	for (int i = N-1; i > 0; i--)
	{
		for (int j = 0; j < N - i; j++)
		{
			cout << " ";
		}
		for (int j = 0; j < i * 2 - 1; j++)
		{
			cout << "*";
		}
		for (int j = 0; j < N - i; j++)
		{
			cout << " ";
		}
		cout << endl;
	}


}

int main()
{
	int N;
	cin >> N;

	solution(N);

	return 0;
}

```

- 오른쪽 공백은 안찍어도 된다.

### 포인터와 레퍼런스

```
#include <iostream>
#include <algorithm>
#include <vector>
#include <queue>
#include <numeric>

using namespace std;

int main()
{
	int x = 3;
	char y = 'A';

	int* ptr1 = &x;
	char* ptr2 = &y;

	cout << "x의 주소: " << &x << ", ptr1: " << ptr1 << endl;
	cout << "y의 주소: " << (void*)&y << ", ptr2: " << (void*)ptr2 << endl;

	return 0;
}
```

- cout << "y의 주소: " << (void*)&y << ", ptr2: " << (void*)ptr2 << endl;

- void로 캐스팅하는 부분이다.
- 기존 char*타입을 cout에 넣으면 ostream은 char*를 문자열 C-String으로 해석해서 그 주소부터 시작하는 문자들을 출력하기때문에 그냥 주소로 보라는 의미로 void\*로 캐스팅합니다.

![](https://velog.velcdn.com/images/kyu_/post/4e6ca628-d29b-4745-8133-f16e751036d2/image.png)

- void로 형변환을 한 경우

![](https://velog.velcdn.com/images/kyu_/post/930b0196-3beb-4534-84fd-bd2816a50f83/image.png)

- void로 형변환을 하지 않은 경우
  - 널문자를 만날때까지 알수없는 쭉 찍힌다
- cout << char\*
  - c문자열로 취급하는 오버로드가 있어서 그 주소부터 \0널이 나올때까지 문자들을 출력하려고 한다.
