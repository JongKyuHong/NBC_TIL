#include <string>
#include <vector>
#include <iostream>

using namespace std;

string convertToBinary(int len){
    string tmp = "";
    
    while (len >= 1){
        tmp = tmp + to_string(len % 2);
        len /= 2;
    }

    return tmp;
}

vector<int> solution(string s) {
    vector<int> answer;
    int zeroCnt = 0;
    int count = 0;
    while (s != "1") {
        count++;
        string tmp = "";
        for (int i = 0; i < s.length(); i++){
            if (s[i] == '0'){
                zeroCnt++;
            } else {
                tmp+=s[i];
            }
        }
        s = convertToBinary(tmp.length());
    }
    
    return {count, zeroCnt};
}