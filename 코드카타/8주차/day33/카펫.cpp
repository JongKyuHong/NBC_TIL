#include <string>
#include <vector>
#include <iostream>

using namespace std;

vector<int> solution(int brown, int yellow) {
    vector<int> answer;
    // brown = 2*w + 2*h - 4;
    // yellow = (w-2) * (h-2);
    
    int total = brown + yellow;
    
    for (int h = 3; h <= total; h++){
        if (total % h == 0){
            int w = total / h;
            if ((brown == 2*w + 2*h - 4) && (yellow = (w-2)*(h-2))){
                return {w, h};
            }
        }
    }
}