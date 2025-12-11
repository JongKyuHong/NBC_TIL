# C++

## 도전실습

### 미니 실습

- [내 풀이] (https://github.com/JongKyuHong/NBC_TIL/blob/main/3%EC%A3%BC%EC%B0%A8/code/day11/Q3.cpp)
- this->Speed는 Speed라는 이름이 중복되었을때 말고는 굳이 쓸필요없다.
- 생성자에서 string& Name으로 매개변수를 받을때 문제가 되었었다. const로 바꾸니까 해결

### 도전 실습

- [내 풀이] (https://github.com/JongKyuHong/NBC_TIL/blob/main/3%EC%A3%BC%EC%B0%A8/code/day11/Q4.cpp)

![](https://velog.velcdn.com/images/kyu_/post/4d7c8c60-9061-48f5-8cbf-98db6b12bc49/image.png)

- 멤버함수만들때 const를 붙이라는 알림이 나옴
- 멤버함수에 const를 붙이는 기준은 객체(this)의 상태를 변경하지 않으면 붙이면 된다.
- getter함수이기 때문에 단순히 반환만 해주어서 const를 붙이게 되었다.

#### cppLint특강

- 특강 듣고 도전실습 코드 돌렸더니 에러가 136개가 나왔다ㄷㄷ

- [개선풀이] (https://github.com/JongKyuHong/NBC_TIL/blob/main/3%EC%A3%BC%EC%B0%A8/code/day11/Q4_1.cpp)
- 개선!

### 도전X도전 실습

![](https://velog.velcdn.com/images/kyu_/post/a9dbbf91-bcc9-4f6d-ba5b-413498543f02/image.png)

- 멤버함수에 const를 쓸때는 호출하는 다른 멤버함수도 const화 되어있어야 한다.

![](https://velog.velcdn.com/images/kyu_/post/37a82408-461b-486d-8bd7-a434400b14a2/image.png)

- 멤버초기화 리스트에서 배열 처음 사용해봄 {}

![](https://velog.velcdn.com/images/kyu_/post/a66a7b77-2eaa-4d8e-9369-64f0ab033e65/image.png)

- Aircraft클래스에서 Pilot crew[2]로 정의되어있는데 만약에 입력이 덜들어 올것을 대비해서 Pilot에 기본 생성자 만들어줌

-[내 풀이] (https://github.com/JongKyuHong/NBC_TIL/blob/main/3%EC%A3%BC%EC%B0%A8/code/day11/Q5.cpp)

## 알고리즘 문제 풀어보기

### 1.

- [FizzBuzz] (https://www.acmicpc.net/problem/28702)

- 문자열을 숫자로 바꾸는것에 대해서 많이 생각하게 되는 문제
- stoi의 경우 숫자가 아니면 error를 발생해서 try/catch문으로 받아야하는데 이거는 알고리즘에서는 크게 의미 없는거 같고
- isdigit은 0~9까지의 일의자리 숫자만 가능하다.
- 그래서 현실적으로는 문자열을 하나씩 뜯어봐서 isdigit을 각각 쓰는게 나아보인다.

- [내 풀이] (https://github.com/JongKyuHong/NBC_TIL/blob/main/3%EC%A3%BC%EC%B0%A8/code/day11/Q1.cpp)

### 2.

- 이 문제는 정렬 메서드 사용법을 익히는데 많이 도움이 되었다.
- 외부함수 사용, 내부에서 사용
- 정렬 조건
- 중복제거를 위해서 set을사용 or 벡터에서 중복제거
- 벡터에서 중복제거를 한다면 입력에서 걸러내기 or 일단 다 입력받고 erase와 unique로 중복 지우기

#### 외부함수 사용

```c++
sort(vec.begin(), vec.end(), [](const string& a, const string& b)
	{
		if (a.length() != b.length())
		{
			return a.length() < b.length();
		}
		return a < b;
	});
```

기존에는 이렇게 사용했다.

```c++
bool compare(string& a, string& b)
{
	if (a.length() != b.length())
	{
		return a.length() < b.length();
	}
	return a < b;
}

sort(vec.begin(), vec.end(), compare);
```

- sort의 세번째인자는 함수포인터(함수 자체)를 요구하기 때문에 인자를 안넘겨줘도 된다.
- vec의 길이가 1이라면 자동으로 sort는 생략된다 그래서 vec.size() > 1 분기를 안넣어줘도 됐음

#### vector로 중복제거 - unique사용

```c++
vec.erase(unique(vec.begin(), vec.end()), vec.end());
sort(vec.begin(), vec.end(), compare);
```

- 기존에는 erase하고 sort하는 방식이였는데
- unique가 바로 옆에 붙은 중복을 제거하고 떨어진 중복은 제거하지 못한다고 한다..

```c++
sort(vec.begin(), vec.end(), compare);
vec.erase(unique(vec.begin(), vec.end()), vec.end());
```

- sort하고 unique + erase로 변경
- 그래도 이 방식이 입력받을때 find(vec.begin(), vec.end(), inp) == vec.end() 이 조건으로 계속 체크하는것보다는 효율적이다.

- [내 풀이] (https://github.com/JongKyuHong/NBC_TIL/blob/main/3%EC%A3%BC%EC%B0%A8/code/day11/Q2.cpp)
