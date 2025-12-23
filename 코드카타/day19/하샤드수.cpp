#include <string>
#include <vector>
#include <string>

using namespace std;

bool solution(int x) {
    string newX = to_string(x);
    int sumV = 0;

    for (int i = 0; i < newX.length(); i++){
        sumV += newX[i]-'0';
    }
    
    if (x % sumV){
        return false;
    } else {
        return true;
    }
}