// Copyright 2025 JongKyu

#include <iostream>
#include <string>
#include <algorithm>

class Item {
 public:
    Item() {}
    Item(const std::string& name, int price) : name_(name), price_(price) {}

    const std::string& GetName() const {
        return name_;
    }
    int GetPrice() const {
        return price_;
    }
    void PrintInfo() const {
        std::cout << "[이름: " << name_ << ", 가격: "
            << price_ << "G]" << std::endl;
    }
 private:
     std::string name_;
     int price_;
};

bool compareItemsByPrice(const Item& a, const Item& b) {
    return a.GetPrice() < b.GetPrice();
}

template<typename T>
class Inventory {
 private:
    T* pItems_;
    int capacity_;
    int size_ = 0;
 public:
    explicit Inventory(int capacity = 10) : capacity_(capacity) {
        if (capacity <= 0) {
            capacity_ = 1;
        }
        pItems_ = new T[capacity_];
    }

    Inventory(const Inventory<T>& other) {
        capacity_ = other.capacity_;
        size_ = other.size_;
        pItems_ = new T[capacity_];
        for (int i = 0; i < size_; i++) {
            pItems_[i] = other.pItems_[i];
        }
        std::cout << "인벤토리 복사 완료" << std::endl;
    }

    void Assign(const Inventory<T>& other) {
        if (this == &other) {
            std::cout << "자기 자신" << std::endl;
            return;
        }

        delete[] pItems_;

        capacity_ = other.capacity_;
        size_ = other.size_;
        pItems_ = new T[capacity_];

        for (int i = 0; i < size_; i++) {
            pItems_[i] = other.pItems_[i];
        }
    }


    ~Inventory() {
        delete[] pItems_;
        pItems_ = nullptr;
    }

    void AddItem(const T& item) {
        if (size_ < capacity_) {
            pItems_[size_++] = item;
        } else {
            std::cout << "인벤토리가 꽉 찼습니다!\n";
            // 도전 기능
            Resize(capacity_ * 2);
            pItems_[size_++] = item;
            std::cout << "인벤토리가 늘어났습니다! 현재 인벤토리 크기 : "
                << capacity_ << "\n";
        }
    }

    void RemoveLastItem() {
        if (size_ > 0) {
            size_ -= 1;
        } else {
            std::cout << "인벤토리가 비어있습니다.\n";
        }
    }

    int GetSize() const {
        return size_;
    }

    int GetCapacity() const {
        return capacity_;
    }

    void PrintAllItems() const {
        std::cout << "--------------------------" << std::endl;
        if (size_ > 0) {
            for (int index = 0; index < size_; index++) {
                pItems_[index].PrintInfo();
            }
        } else {
            std::cout << "(비어있음)\n";
        }
        std::cout << "--------------------------" << std::endl;
    }

    void Resize(int newCapacity) {
        T* newItems = new T[newCapacity];

        int targetSize = (size_ < newCapacity) ? size_ : newCapacity;
        for (int i = 0; i < targetSize; i++) {
            newItems[i] = pItems_[i];
        }

        delete[] pItems_;

        pItems_ = newItems;

        capacity_ = newCapacity;

        if (size_ > newCapacity) {
            size_ = newCapacity;
        }

        std::cout << "인벤토리 리사이즈" << std::endl;
    }

    void SortItems() {
        std::sort(pItems_, pItems_ + size_, compareItemsByPrice);
    }
};


int main() {
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

    return 0;
}
