#include <string>
#include <vector>
#include <queue>
#include <stack>
#include <iostream>

using namespace std;

int solution(vector<int> order) {
    int answer = 0;
    
    queue<int> q;
    stack<int> st;
    
    for (int i = 1; i <= order.size(); i++){
        q.push(i);
    }
    
    for (int i = 0; i < order.size(); i++){
        int target = order[i];
        if (q.empty() || target < q.front()){
            // 스택을 본다.
            if (!st.empty()){
                int tmp = st.top();
                if (tmp == target){
                    answer++;
                    st.pop();
                } else {
                    break;
                }
            }
        } else {
            while (!q.empty()){
                int tmp = q.front();
                if (tmp == target){
                    answer++;
                    q.pop();
                    break;
                } else {
                    st.push(tmp);
                    q.pop();
                }
            }
        }
        
    }
    
    return answer;
}