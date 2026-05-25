## 1번과제

- cpplint를 설정해서 구글 C++에 맞게 변경

### 문자열 입력 걸러내기

```c++
// 잘못된 입력 걸러내는 함수
void ClearBadInput() {
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

// HP/MP 설정 함수
void SetHPMP(int* statusPtr) {
    while (true) {
        cout << "HP와 MP를 입력해주세요 : ";
        int InpHP;
        int InpMP;
        if (!(cin >> InpHP >> InpMP)) {
            ClearBadInput();
            cout << "숫자를 입력해주세요.\n";
            continue;
        }

        if (InpHP <= 50 || InpMP <= 50) {
            cout << "HP나 MP의 값이 너무 작습니다. 다시 입력해주세요.\n";
            continue;
        } else {
            statusPtr[HP] = InpHP;
            statusPtr[MP] = InpMP;
            return;
        }
    }
}
```

- HP, MP, 공격력, 방어력을 입력할때 while문을 사용해서 숫자가 들어오지않으면 무한루프를 도는 문제가있었는데
- ClearBadInput함수를 추가해서 해결
- cin >> InpHP >> InpMP가 istream&형태의 스트림을 반환하는데 조건문에서 이는 현재 스트림이 정상인지로 평가된다.
- 숫자읽기가 실패하면 바로 조건안으로 들어가서 ClearBadInput을 만난다.
- cin.clear로 입력 스트림을 정상으로 돌리고
- cin.ignore(numeric_limits<streamsize>::max(), '\n')를 통해서 입력 버퍼에 남아있는 잘못된 토큰을 다음 줄바꿈까지 통째로 버리는 역할을 한다.
