#include <string>
#include <vector>
#include <unordered_map>

using namespace std;

int solution(vector<int> topping) {
    int answer = 0;
    unordered_map<int, int> bro;
    unordered_map<int, int> yBro;
    
    for (int i = 0; i < topping.size(); i++){
        yBro[topping[i]]++;
    }
    
    for (int i = 0; i < topping.size(); i++){
        bro[topping[i]]++;
        yBro[topping[i]]--;
        if (yBro[topping[i]] == 0){
            yBro.erase(topping[i]);
        }
        if (bro.size() == yBro.size()){
            answer++;
        }
    }
    
    return answer;
}