# C++

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
