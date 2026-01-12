# 코드카타

## 1. 둘만의 암호

```c++
#include <string>
#include <vector>
#include <iostream>

using namespace std;

string solution(string s, string skip, int index) {
    string answer = "";
    vector<char> vec(26, 'a');
    vector<bool> isSkip(26, false);
    for (int i = 0; i < 26; i++){
        vec[i] = 'a' + i;
    }

    for (int i = 0; i < 26; i++){
        if (skip.find(vec[i]) != string::npos){
            isSkip[i] = true;
        }
    }

    for (int i = 0; i < s.length(); i++){
        char current = s[i];
        int tmp = 0;
        while (tmp < index){
            current++;
            if (current > 'z'){
                current = 'a';
            }
            if (!isSkip[current-'a']){
                tmp++;
            }
        }
        answer += current;
    }

    return answer;
}
```

- 문자를 다루는데 굉장히 유용한 문제였던것 같다.
  - char형 current를 그냥 current++해서 사용한다던지
  - char형 벡터 vec에 'a'+i : 즉 문자더하기 int를 해서 문자로 넣는다던지
- isSkip이라는 skip에 들어가는 알파벳들을 빠르게 판단할수있는 벡터를 하나 만들어서 사용했다.
