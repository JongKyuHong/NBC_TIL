#include <string>
#include <vector>
#include <string>

using namespace std;

string solution(string s) {
    int len = s.length();
    string tmp;
    int index = s.length() / 2;
    if (len % 2){
        tmp = s.substr(index, 1);    
    } else {
        tmp = s.substr(index-1, 2);
    }
    
    return tmp;
}