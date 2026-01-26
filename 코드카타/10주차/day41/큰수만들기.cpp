#include <string>
#include <vector>
#include <stack>

using namespace std;

string solution(string number, int k) {
    string answer = "";
    
    stack<char> st;
    int index = 0;
    while (index < number.length()){
        while (!st.empty() && st.top() < number[index]){
            st.pop();
            k--;
            if (k == 0){
                break;
            }
        }
        if (k == 0){
            break;
        }
        
        st.push(number[index]);    
        index++;
    }
    
    // 테스트케이스가 "10", 1이 들어왔을때 "10"이 출력됨 k가 남았을때 처리를 안함
    // 스택 위쪽이 작은값이므로 k개수만큼 빼줌
    while (k > 0 && !st.empty()) {
        st.pop();
        k--;
    }
    
    while(!st.empty()){
        answer = st.top() + answer;
        st.pop();
    }
    
    for (int i = index; i < number.length(); i++){
        answer += number[i];
    }
    
    return answer;
}