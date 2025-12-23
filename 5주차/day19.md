## 코드카타

### string에서 알고리즘, 이터레이터 쓰기

```c++
long long solution(long long n) {
long long answer = 0;
string newN = to_string(n);
sort(newN.begin(), newN.end(), greater<char>());
    return stoll(newN);
}
```

- 정수 내림차순으로 배치하기 문제는 정수n의 각 자릿수를 내림차순 정렬하고 그를 반환하는 문제였다.
- 이때 sort알고리즘과 이터레이터를 사용할 수 있다.
- string은 내부적으로 char배열을 관리하므로 가능하다.
- 내림차순으로 쉽게 변경할때 greater<T>()를 사용하도록 하자
