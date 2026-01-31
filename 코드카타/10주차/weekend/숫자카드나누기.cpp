#include <string>
#include <vector>
#include <numeric>
#include <iostream>
#include <cmath>
#include <algorithm>

using namespace std;

vector<int> find(int value){
    vector<int> tmp;
    tmp.push_back(value);
    for (int i = sqrt(value); i > 1; i--){
        if (value % i == 0){
            tmp.push_back(value);
        }
    }

    return tmp;
}

int solution(vector<int> arrayA, vector<int> arrayB) {
    int answer = 0;
    
    int tmp = arrayA[0];
    for (int i = 1; i < arrayA.size(); i++){
        tmp = gcd(tmp, arrayA[i]);
    }
    
   
    
    int tmp2 = arrayB[0];
    for (int i = 1; i < arrayB.size(); i++){
        tmp2 = gcd(tmp2, arrayB[i]);
    }
    
    vector<int> tmp1vec = find(tmp);
    vector<int> tmp2vec = find(tmp2);
    
    int maxV = 0;
    int index1 = 0;
    while (index1 < tmp1vec.size()){
        bool flag = false;
        int target = tmp1vec[index1];
        for (int i = 0; i < arrayB.size(); i++){
            if (arrayB[i] % target == 0){
                flag = true;
                break;
            }
        }
        if (flag) {
            index1++;
        } else {
            maxV = max(maxV, target);
            break;
        }
    }
    
    int index2 = 0;
    while (index2 < tmp2vec.size()){
        bool flag = false;
        int target = tmp2vec[index2];
        for (int i = 0; i < arrayA.size(); i++){
            if (arrayA[i] % target == 0){
                flag = true;
                break;
            }
        }
        if (flag) {
            index2++;
        } else {
            maxV = max(maxV, target);
            break;
        }
    }
    
    return maxV;
}