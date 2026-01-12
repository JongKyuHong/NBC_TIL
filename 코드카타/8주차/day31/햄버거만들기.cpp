#include <string>
#include <vector>
#include <stack>

using namespace std;

int solution(vector<int> ingredient) {
    int answer = 0;
    vector<int> st;
    vector<int> vec;

    int index = 0;
    for (int i = 0; i < ingredient.size(); i++){
        st.push_back(ingredient[i]);
        if (st.size() >= 4){
            int len = st.size() - 1;
            if (st[len] == 1 && st[len-1] == 3 && st[len-2] == 2 && st[len-3] == 1){
                st.pop_back();
                st.pop_back();
                st.pop_back();
                st.pop_back();
                answer++;
            }
        }
    }
    
    
    return answer;
}