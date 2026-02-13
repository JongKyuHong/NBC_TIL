# 프로젝트

## 앉기 버그를 고쳐보자

- 1번 영상

- 속도 안느려짐
- 태그 안없어짐
  - 다시 못앉음
- Ctrl누르는 동안 앉아야하는데 자기가 알아서 일어남

- 로그사진
- 로그에서 수상한 부분이 보였다.
- 앉기기능을 거부한다.

### Can Crouch설정 On

- BP_PlayerCharacter에서 Can Crouch를 켜주었다.
- 2번 영상
  - 앉기상황일때 속도가 줄어들도 debug 오버랩도 작아진걸 볼 수 있었다.
  - 하지만 여전히 태그가 안없어진다.

### 키 떼기를 인식하는가??

```c++
void APlayerCharacter::OnCrouch(const FInputActionValue& Value)
{
    bool bIsPressed = Value.Get<bool>();
    UAbilitySystemComponent* ASC = GetAbilitySystemComponent();
    if (!ASC) return;
    if (bIsPressed)
    {
        FGameplayTagContainer AbilityTags;
        AbilityTags.AddTag(FGameplayTag::RequestGameplayTag(FName("Ability.Player.Crouch")));
        ASC->TryActivateAbilitiesByTag((AbilityTags));
    }
    else
    {
        // 이벤트와 함께 전달되는 Payload(정보)
        // 이벤트 발생주체, 대상등이 들어있음
        FGameplayEventData Payload;
        ASC->HandleGameplayEvent(FGameplayTag::RequestGameplayTag(FName("State.Crouch.End")), &Payload);
    }
}
```

- 캐릭터에서 매핑되어있는 OnCrouch함수에서 bIsPressed라는 값을 통해서 눌렀는지 뗐는지를 확인
- TryActivateAbilitiesByTag는 태그를 가진 어빌리티를 실행하라는 명령
- HandleGameplayEvent는 이미 실행중인 어빌리티에게 신호를 보내는 것 (키가 떼어졌다)
- State.Crouch.End라는 상태 태그를 만들어서 키를 떼었을때의 태그도 만들었다.

- 앉기잘되는 3번째영상

### 앉기가 풀림

- anim montage에서 Loop Section설정한거 보여주기

### GE가 Montage주기마다 계속 실행됨

- 4번째 영상

###

# 코드카타

## 이모티콘 할인행사

- [이모티콘 할인행사](https://school.programmers.co.kr/learn/courses/30/lessons/150368?language=cpp)

```c++
#include <string>
#include <vector>
#include <algorithm>
#include <iostream>

using namespace std;

int maxPlus = 0;
int maxMoney = 0;
vector<int> disCounts;

vector<int> calculate(){

}

void dfs(int depth, vector<vector<int>>& users, vector<int>& emoticons){
    if (depth == emoticons.size()){
        int totalPlus = 0;
        int totalMoney = 0;
        for (auto user : users){
            int sumV = 0;
            for (int i = 0; i < emoticons.size(); i++){
                if (disCounts[i] >= user[0]){
                    sumV += (emoticons[i]*(100-disCounts[i])) / 100;
                }
            }
            if (sumV >= user[1]){
                sumV = 0;
                totalPlus++;
            } else {
                totalMoney += sumV;
            }
        }
        if (maxPlus < totalPlus){
            maxPlus = totalPlus;
            maxMoney = totalMoney;
        } else if (maxPlus == totalPlus){
            maxMoney = max(maxMoney, totalMoney);
        }
        return;
    }

    for (int i = 0; i < 4; i++){
        disCounts.push_back((i+1)*10);
        dfs(depth+1, users, emoticons);
        disCounts.pop_back();
    }
}

vector<int> solution(vector<vector<int>> users, vector<int> emoticons) {
    vector<int> tmp;

    dfs(0, users, emoticons);

    return {maxPlus, maxMoney};
}
```

- 별로 어려운문제는 아니였는데 처음에 접근방법을 잘못잡아서 포기하고 다시 돌아오는게 오래걸렸다.
- 전역변수로 배열을 만들고 할인율만 저장하여 계산하였다.
