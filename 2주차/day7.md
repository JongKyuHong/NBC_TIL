# C++

### 도전 실습

```
#include <iostream>

using namespace std;

void calculator(int calType, int a, int b)
{
	if (calType == 1) // 덧셈
	{
		cout << a << " + " << b << " = " << a + b << "\n";
	}
	else if (calType == 2) // 뺄셈
	{
		cout << a << " - " << b << " = " << a - b << "\n";
	}
	else if (calType == 3) // 곱셈
	{
		cout << a << " * " << b << " = " << a * b << "\n";
	}
	else if (calType == 4) // 나눗셈
	{
		cout << a << " / " << b << " = " << a / b << "\n";
	}
	else // 그 이외의 숫자가 들어오면
	{
		cout << "잘못된 입력입니다." << "\n";
	}
}

int main()
{
	while (true)
	{
		cout << "사칙 연산 모드를 설정해주세요." << "\n";
		cout << "1) 덧셈" << "\n";
		cout << "2) 뺄셈" << "\n";
		cout << "3) 곱셈" << "\n";
		cout << "4) 나눗셈" << "\n";
		cout << "-1) 나가기" << "\n";

		int state = 0, inp1 = 0, inp2 = 0;

		// 계산 뭐할지 입력
		cout << "원하는 계산 입력해주세요 : ";
		cin >> state;

		if (state == -1)
		{
			break;
		}

		// 두 수 입력
		cout << "첫 번째 수 입력 : ";
		cin >> inp1;
		cout << "두 번째 수 입력 : ";
		cin >> inp2;

		// 계산기 호출
		calculator(state, inp1, inp2);
		cout << endl;
	}

	return 0;
}
```

### 도전x도전 실습

- 첫번째 코드
- [내 풀이](https://github.com/JongKyuHong/NBC_TIL/blob/main/2%EC%A3%BC%EC%B0%A8/code/day7Q1.cpp)

- 모든 분기를 만들었다. 코드더럽
- 괄호에 따른 연산 우선순위를 적용하려면 스택을 사용해야 하기때문에 구조를 모두 바꿔야한다.

#### 괄호추가 1차

- [내 풀이](https://github.com/JongKyuHong/NBC_TIL/blob/main/2%EC%A3%BC%EC%B0%A8/code/day7Q2.cpp)
- 5 + 7 (8*7) 처럼 숫자와 괄호가 붙어있는 경우는 제대로 판별을 못해서 숫자 + 괄호가 붙어있는 부분은 *를 붙여준다던지 하는 식으로 계산해야 할것 같다.

#### 코드 작성하면서 느낀점

- sstream, istringstream, getline조합으로 문자열 한줄을 받고 공백을 제거하면서 배열형태로 받을 수 있다.

```
istringstream ss(inp);
vector<string> tokens;
string token;

while (ss >> token)
{
tokens.push_back(token);
}
```

- goto 문법
  - goto 이동할곳;
  - 이동할곳 :
  - 실행할내용;
