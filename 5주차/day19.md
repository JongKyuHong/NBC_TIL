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

### 수의 범위

```c++
int solution(int num) {
    int answer = 0;
    long long tmp = num;
    if (num == 1){
        return 0;
    }
    while (true){
        if (tmp % 2 == 0){
            tmp /= 2;
        } else {
            tmp = (tmp * 3) + 1;
        }
        answer++;
        if (answer == 500){
            return -1;
        }
        if (tmp == 1){
            break;
        }
    }
    return answer;
}
```

- 콜라츠 추측문제를 푸는데 num이 int형이라서 수의 범위때문에 올바른 값이 리턴이 되지않았다.
- long long에다가 넣어주니 해결되었다. 문제의 조건을 보고 수의 범위를 생각하면서 해야될듯

### find활용방법