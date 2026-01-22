#include <string>
#include <vector>
#include <iostream>
#include <algorithm>
#include <cmath>
#include <unordered_set>

using namespace std;

bool isPrime(int tmp){
    if (tmp < 2) return false;

    for (int i = 2; i <= sqrt(tmp); i++){
        if (tmp % i == 0) return false;
    }
    return true;
}

int solution(string numbers) {
    int n = numbers.size();
    unordered_set<int> ss;
    
    sort(numbers.begin(), numbers.end());
    
    do{
        string tmp = "";
        for (int i = 1; i <= n; i++){
            int num = stoi(numbers.substr(0, i));   
            if (isPrime(num)){
                ss.insert(num);
            }
        }
    } while(next_permutation(numbers.begin(), numbers.end()));
    
    return ss.size();
}