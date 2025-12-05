#include <iostream>
using namespace std;

int main() {
    int x = 3;    // 정수형 변수 x 선언
    int* ptr = &x; // 포인터 ptr에 x의 주소 저장

    cout << "x의 값: " << x << endl;
    cout << "x의 주소: " << &x << endl;
    cout << "ptr의 값(저장된 주소): " << ptr << endl;

    return 0;
}