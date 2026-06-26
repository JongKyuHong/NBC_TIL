// https://school.programmers.co.kr/learn/courses/30/lessons/70129

#include <string>
#include <vector>
#include <iostream>
#include <algorithm>
#include <bitset>

using namespace std;

vector<int> solution(string s) {
    vector<int> answer;
    int oneCount = 0;
    int zeroCount = 0;
    int transformCount = 0;
    
    while (s != "1"){
        transformCount++;
        int len = s.size();
        oneCount = count(s.begin(), s.end(), '1');
        zeroCount += len - oneCount;
        string ss = bitset<32>(oneCount).to_string();
        s = ss.substr(ss.find('1'));
    }
    
    return {transformCount, zeroCount};
}