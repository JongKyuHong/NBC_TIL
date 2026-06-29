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


인런 진입할때 A라는 바디 파츠장착 후 진입
인런중에 B라는 다른 바디 파츠로 교체
다시 아웃런으로 돌아왔을때 B파츠 말고 A파츠로 돌아와야 하는지

