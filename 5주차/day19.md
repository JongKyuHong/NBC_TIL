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

```c++
#include <algorithm>

string solution(vector<string> seoul) {
    string answer = "";
    auto it = find(seoul.begin(), seoul.end(), "Kim");
    answer = "김서방은 " + to_string(it-seoul.begin()) + "에 있다";
    return answer;
}
```

- 서울에서 김서방 찾기문제를 풀때 처음에 vector의 find활용방법이 헷갈렸다.
- 이번기회에 한번 정리해야겠다.

- vector, list는 altorithm헤더의 find를 쓴다.
  - find(vec.begin(), vec.end(), 찾는값);
  - 못찾으면 vec.end()반환
- 멤버 함수 find() : 보통 set, map, multimap, multiset, unordered_set, unordered_map에서 사용
  - map.find(키 혹은 값)
  - 못찾으면 map.end()반환
- string의 find
  - str.find("값");
  - 못찾으면 string::npos반환, 찾아도 다른애들은 반환타입이 이터레이터인데 얘는 size_t

### 벡터에서 가장 작은 값 찾기

```c++
#include <algorithm>

using namespace std;

vector<int> solution(vector<int> arr) {
    auto it = min_element(arr.begin(), arr.end());
    arr.erase(arr.begin() + distance(arr.begin(), it));
    if (arr.empty()){
        arr.push_back(-1);
    }
    return arr;
}
```

- 알고리즘 헤더의 min_element를 활용하면 된다.
- 가장 작은 값의 이터를 반환해주고 distance를 통해서 인덱스를 찾고 해당 인덱스의 값을 삭제해준다.
