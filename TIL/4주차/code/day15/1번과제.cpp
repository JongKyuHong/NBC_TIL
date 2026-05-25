// Copyright 2025 JongKyu

#include <iostream>
#include <string>
#include <algorithm>

class Item {
 public:
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
        if (size_ > 0) {
            for (int index = 0; index < size_; index++) {
                pItems_[index].PrintInfo();
            }
        } else {
            std::cout << "(비어있음)\n";
        }
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
    }

    void SortItems() {
        std::sort(pItems_, pItems_ + size_, compareItemsByPrice);
    }
};


int main() {

    return 0;
}
