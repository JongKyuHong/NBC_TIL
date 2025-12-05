#include <iostream>
using namespace std;

int main() {
    const int SIZE = 1000000; // 1,000,000개의 정수 (약 4MB)
    int arr1[SIZE]; 
    int arr2[SIZE];

    for (int i = 0; i < SIZE; i++) {
        arr2[i] = arr1[i];
    }

    cout << "배열 복사 완료" << endl;

    return 0;
}