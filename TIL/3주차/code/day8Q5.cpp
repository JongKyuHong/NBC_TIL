#include <iostream>
using namespace std;

class MyClass {
private:
    int* ptr;

public:
    MyClass() {
        ptr = new int(10);
        cout << "메모리 할당 완료!" << endl;
    }

    ~MyClass() {
        delete ptr;
        ptr = nullptr;
    }

    void print() const {
        cout << "값: " << *ptr << endl;
    }
};

int main() {
    MyClass obj;
    obj.print();

    return 0;
}
