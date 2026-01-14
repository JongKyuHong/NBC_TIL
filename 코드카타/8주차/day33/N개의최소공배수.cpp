#include <string>
#include <vector>
#include <iostream>

using namespace std;

int gcd(int a, int b){
    while (b != 0){
        int tmp = a % b;
        a = b;
        b = tmp;
    }
    
    return a;
}

int lcm(int a, int b){
    return (a*b)/gcd(a, b);
}

int solution(vector<int> arr) {
    int answer = 0;
    
    int index = 1;
    int current = arr[0];
    while (index < arr.size()){
        int tmp = lcm(current, arr[index]);
        current = tmp;
        index++;
    }
    
    return current;
}