#include <string>
#include <vector>
#include <set>
#include <iostream>

using namespace std;

int solution(vector<int> elements) {
    int answer = 0;
    set<int>  accum;
    
    int len = elements.size();
    
    for (int i = 0; i < len; i++){
        elements.push_back(elements[i]);
        accum.insert(elements[i]);
    }
    for (int i = 2; i <= len; i++){
        int start = 0;
        int end = 0;
        int accumulate = 0;
        while (end < elements.size()){
            while (end - start < i){
                accumulate += elements[end++];
            }
            accum.insert(accumulate);
            accumulate -= elements[start];
            start++;
        }
    }

    return accum.size();
}