// https://school.programmers.co.kr/learn/courses/30/lessons/42577?language=cpp

#include <string>
#include <vector>
#include <algorithm>
#include <unordered_set>
#include <iostream>

using namespace std;

bool solution(vector<string> phone_book) {
    // 접두어인 경우를 판단하라
    // phone_book을 길이 순으로 정렬을 해야하고, 자기보다 길이가 작은 단어를 포함하는지를 확인해야 한다?
    // 길이가 백만이라서 2중for문은 불가능하다.
    // 문자열이 어떤 값을 포함하고있는지, 문자열에서 특정 인덱스만 추출하는 방법
    
    // 1. 전화번호부를 입력값길이 기준으로 정렬
    sort(phone_book.begin(), phone_book.end(), [](auto& a, auto& b){
       if (a.length() != b.length()) return a.length() < b.length(); 
    });
    
    // 2. for문으로 순회한다. 그리고 us같은데에다가 넣어놓음
    unordered_set<string> us;
    
    // 3. for문 순회할때 us를 돌렸는데 말고 그냥 문자열의 부분 문자열을 돌리는게 더 나을수도?
    for (const string& phoneNumber : phone_book){
        string tmp = "";
        int plen = phoneNumber.length();
        for (int i = 0; i < plen; ++i){
            tmp += phoneNumber[i];
            if (us.find(tmp) != us.end()){
                return false;
            }
        }
        us.insert(phoneNumber);
    }
    
    return true;
}