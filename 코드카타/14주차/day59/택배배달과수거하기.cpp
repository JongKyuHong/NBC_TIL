#include <string>
#include <vector>
#include <iostream>

using namespace std;

long long solution(int cap, int n, vector<int> deliveries, vector<int> pickups) {
    long long answer = 0;
    
    // 먼곳부터 다녀오면 된다.
    int dIndex = deliveries.size()-1;
    int pIndex = pickups.size()-1;
    while (true){
        if (deliveries[dIndex] == 0) {
            deliveries.pop_back();
            dIndex--;
        }
        if (pickups[pIndex] == 0) {
            pickups.pop_back();
            pIndex--;
        }
        if (deliveries[dIndex] != 0 && pickups[pIndex] != 0) break;
    }
    
    // 근데 가면서 배달했다고 치면 상관없는거 같기도?
    
    while (true){
        if (dIndex < 0 && pIndex < 0) break;
        int tmp = cap;
        answer += max(dIndex+1, pIndex+1) * 2;
        while (true) {
            if (dIndex < 0) break;
            while (dIndex >= 0 && deliveries[dIndex] == 0) {
                deliveries.pop_back();
                dIndex--;
            }
            if (tmp <= 0) break;
    
            if (deliveries[dIndex] != 0) {
                if (tmp >= deliveries[dIndex]) {
                    tmp -= deliveries[dIndex];
                    deliveries.pop_back();
                    dIndex--;
                } else {
                    deliveries[dIndex] -= tmp;
                    tmp = 0;
                }
            } else {
                deliveries.pop_back();
                dIndex--;
            }
        }

        tmp = 0;
        while (true) {
            if (pIndex < 0) break;
            while (pIndex >= 0 && pickups[pIndex] == 0) {
                pickups.pop_back();
                pIndex--;
            }
            if (tmp == cap) break;
            
            if (pickups[pIndex] != 0) {
                if (tmp + pickups[pIndex] <= cap) {
                    tmp += pickups[pIndex];
                    pickups.pop_back();
                    pIndex--;
                } else {
                    pickups[pIndex] -= (cap - tmp);
                    tmp = cap;
                }
            } else {
                pickups.pop_back();
                pIndex--;
            }
        }
    }
    
    return answer;
}