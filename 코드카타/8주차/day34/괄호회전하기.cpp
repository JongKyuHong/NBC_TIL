#include <string>
#include <vector>
#include <stack>

using namespace std;

int solution(string s) {
    int answer = 0;
    
    for (int i = 0; i < s.length(); i++){
        stack<char> st;
        for (int j = 0; j < s.length(); j++){
            int current = s[j];
            if (st.empty()){
                st.push(current);
            } else {
                if (current == '[' || current == '{' || current == '('){
                    st.push(current);
                } else if (current == ']'){
                    if (st.top() == '['){
                        st.pop();
                    } else {
                        st.push(current);
                    }
                } else if (current == '}'){
                    if (st.top() == '{'){
                        st.pop();
                    } else {
                        st.push(current);
                    }
                } else if (current == ')'){
                    if (st.top() == '('){
                        st.pop();
                    } else {
                        st.push(current);
                    }
                }
            }
        }
        
        if (st.empty()){
            answer++;
        }
        char tmp = s[0];
        s.erase(0, 1);
        s += string(1, tmp);
    }
    
    return answer;
}