#include <string>
#include <vector>
#include <string>
#include <stack>
#include <iostream>

using namespace std;

int solution(int storey) {
    int answer = 0;

    string target = to_string(storey);
    stack<int> st;
    
    for (char t : target){
        st.push(t - '0');
    }
    
    while (!st.empty()){
        int s = st.top();
        st.pop();
        if (s > 5 && s < 10){
            answer += 10 - s;
            if (!st.empty()){
                st.top() = st.top() + 1;
            } else {
                answer += 1;
            }
        } else if (s < 5){
            answer += s;
        } else if (s == 5){
            if (!st.empty()){
                if (st.top() >= 5){
                    answer += 10 - s;
                    st.top() = st.top() + 1;
                } else {
                    answer += s;
                }
            } else {
                answer += s;   
            }
        } else if (s == 10){
            if (!st.empty()){
                st.top() = st.top() + 1;
            } else {
                answer += 1;
            }
        }
    }
    
    return answer;
}