![](https://velog.velcdn.com/images/kyu_/post/011a8cb2-0d21-428e-a838-a06f3a8a355a/image.png)

## 팀프로젝트

첫 팀프로젝트 과제가 발제되었다.
팀의 팀장을 맡게되었는데 게임을 주제로 팀프로젝트 하는게 처음이라 어떤것부터 해야하는지 감이 안오는 상황이다.
그래도 지금 이렇게 경험해놓으면 나중에 팀 프로젝트할때 조금 더 수월할것이라고 기대하고있다. 우선 내일 회의때 이야기해볼법한 아이디어 같은것들을 가져가야 될 것 같다.

## 게임 개발자를 위한 C++ 문법

### 3주차 숙제

- Observer패턴에 대해서 구현해보는 숙제였다.

#### Customer 클래스

```c++
class Customer {
public:
    virtual void update(const string& status) = 0;
};
```

- 옵저버 역할을 담당하는 추상클래스
- 얘를 상속받아서 여러 종류의 고객클래스들을 만든다.

#### DeliveryService 클래스

```c++
class DeliveryService {
private:
    vector<Customer*> customers;
    string currentStatus;

public:
    void addCustomer(Customer* customer) {
        customers.push_back(customer);
    }

    void removeCustomer(Customer* customer) {
        auto it = find(customers.begin(), customers.end(), customer);
        if (it != customers.end()) {
            customers.erase(it);
        }
    }

    void updateStatus(const string& status) {
        currentStatus = status;
        // cout << "Updating status : " << status << endl;
        notifyCustomers();
    }

    void notifyCustomers() {
        for (Customer* customer : customers) {
            customer->update(currentStatus);
        }
    }
};
```

- customers는 고객 리스트
- removeCustomer에서 algorithm헤더의 find와 vector의 erase를 통해서 입력받은 고객을 지워주었다.
- updateStatus는 상태를 업데이트하고, notifyCustomers는 모든 고객에게 상태를 알린다.

## 코드카타

- [푸드 파이트 대회](https://school.programmers.co.kr/learn/courses/30/lessons/134240)

```c++
#include <string>
#include <vector>
#include <algorithm>

using namespace std;

string solution(vector<int> food) {
    string answer = "";

    for (int i = 1; i < food.size(); i++){
        if (food[i] / 2 > 0){
            for (int j = 0; j < food[i] / 2; j++){
                answer += to_string(i);
            }
        }
    }

    string result = answer;
    reverse(answer.begin(), answer.end());
    result = result + "0" + answer;

    return result;
}
```

- 문자열을 뒤집어서 붙여주는 형식으로 풀었는데 문자열을 뒤집는 방법을 몰랐다.
- algorithm헤더의 reverse + 이터레이터를 사용해서 문자열을 뒤집을 수 있었다.
  - reverse는 문자열 원본을 뒤집기때문에 리턴이 없다.
