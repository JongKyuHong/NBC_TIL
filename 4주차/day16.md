![](https://velog.velcdn.com/images/kyu_/post/da8b4926-105b-40bc-ab71-5372f098c9d8/image.png)

# C++

## 3번과제

### 구현

#### 1.

`Inventory<Item> myInventory(5)`로 Inventory 객체 만들때 Item기본 생성자가 없다고 하는 오류가 났다.
인벤토리의 생성자를 보면

```c++
 explicit Inventory(int capacity = 10) : capacity_(capacity) {
     if (capacity <= 0) {
         capacity_ = 1;
     }
     pItems_ = new T[capacity_];
 }
```

마지막줄에 pItems* = new T[capacity*]가 존재하는데 T가 Item이므로 기본생성자가 필요했다.
기존 Item의 생성자는 `Item(const std::string& name, int price) : name_(name), price_(price) {}` 이렇게 이름과 값을 입력받아서 초기화하는 생성자만 있었는데 기본 생성자`Item(){}`를 만들어서 해결하였다.

#### 2.

테스트 시나리오는 특히 대입연산자, 복사 생성자는 ai의 도움을 받아서 모든 케이스를 다 테스트 해보려고 했다.

```c++
std::cout << "시작" << std::endl;
std::cout << "기본 인벤토리 5칸" << std::endl;
Inventory<Item> myInventory(5);
// ================================
// 1. AddItem확인
// ================================
std::cout << "기본 아이템 획득" << std::endl;
myInventory.AddItem(Item("몽둥이", 7));
myInventory.AddItem(Item("흰티", 6));
myInventory.AddItem(Item("반바지", 5));

myInventory.PrintAllItems();

// ================================
// 2. 인벤토리 크기
// ================================
std::cout << "현재 인벤토리 크기 : " << myInventory.GetCapacity() << std::endl;

// ================================
// 3. 현재아이템 갯수
// ================================
std::cout << "현재 아이템 개수 : " << myInventory.GetSize() << std::endl;

// ================================
// 4. 인벤토리 리사이즈
// ================================
std::cout << "달팽이를 잡았다! 전리품 획득" << std::endl;
myInventory.AddItem(Item("달팽이의 껍질", 1));
std::cout << "파란달팽이를 잡았다! 전리품 획득" << std::endl;
myInventory.AddItem(Item("파란 달팽이의 껍질", 3));
myInventory.PrintAllItems();

myInventory.Resize(3);
myInventory.PrintAllItems();

// ================================
// 5. 인벤토리 확장되는지 확인
// ================================
std::cout << "빨간달팽이를 잡았다! 전리품 획득" << std::endl;
myInventory.AddItem(Item("빨간 달팽이의 껍질", 5));
myInventory.PrintAllItems();

// ================================
// 6. 마지막 원소 삭제
// ================================

myInventory.RemoveLastItem();
myInventory.PrintAllItems();

// ================================
// 7. 정렬
// ================================

myInventory.SortItems();
myInventory.PrintAllItems();

// ================================
// 8, 복사 생성자 테스트
// ================================

Inventory<Item> clone = myInventory;

std::cout << "복사 인벤토리 " << std::endl;
clone.PrintAllItems();

// 깊은 복사 테스트

myInventory.RemoveLastItem();
std::cout << "원본" << std::endl;
myInventory.PrintAllItems();

std::cout << "복사본" << std::endl;
clone.PrintAllItems();

// ================================
// 9, 대입 연산자 테스트
// ================================

Inventory<Item> other(3);
other.AddItem(Item("해킹템", 1000));

std::cout << "기본 내 인벤토리" << std::endl;
myInventory.PrintAllItems();

myInventory = other;

std::cout << "해킹 후 내 인벤토리" << std::endl;
myInventory.PrintAllItems();


// ================================
// 10. 비어있음 테스트
// ================================

myInventory.RemoveLastItem();
myInventory.PrintAllItems();
```

- [내 코드](https://github.com/JongKyuHong/NBC_TIL/blob/main/3%EC%A3%BC%EC%B0%A8/code/day16/3번과제.cpp)

## 4번과제

### 체크포인트 적기

- [ ] 제공된 코드 분석
- [ ] 검색기능 추가하기
  - [ ] 물약 이름으로 검색 가능
  - [ ] 재료로 검색 가능, 특정재료가 포함된 모든 레시피 찾기
- [ ] 물약은 초기3개
- [ ] 지급 및 반환

  - [ ] 특정 물약의 현재 재고 수량
  - [ ] 물약이름으로 재고 검색, 재고 1개 이상일 경우 모험가에게 지급할 수 있어야함
  - [ ] 레시피 재료로 물약들의 재고 검색, 재고 1개이상인 경우 지급할 수 있어야함
  - [ ] 모험가에게 지급한 물약 공병 반환받기
  - [ ] 최대 재고 3개, 반환 시 3개 초과 불가하다.

- [ ] 어떤 의도로 코드를 구현했는지 정리
- [ ] 수정 시 기존 코드의 수정을 최소화하면서 수정할 수 있는 방안이 있는지 고려했는가
- [ ] SOLID 원칙을 잘 준수하고 있는가
