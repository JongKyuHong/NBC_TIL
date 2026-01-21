#include <string>
#include <vector>
#include <stack>

using namespace std;

vector<int> solution(vector<int> numbers) {
    vector<int> answer(numbers.size(), -1);
    stack<pair<int, int>> ss;

    // 바로 뒤
    // 백만의 제곱은 일조 당연히 불가능 보통 일억번 연산까지 가능
    // 다시말해 거의 한번의 순회정도로 풀어야한다.
    // 보통 이런 문제는 스택으로 푼다.
    // 비어있으면 첫번째 원소를 넣고, 뒤 숫자들이 자기보다 크면 스택을 뺀다? , 실제 값을 저장하는 스택과, 인덱스를 저장하는 어떤것이 필요
    for (int i = 0; i < numbers.size(); i++) {
        if (ss.empty()) {
            ss.push({ numbers[i], i });
        } else {
            if (ss.top().first >= numbers[i]) {
                ss.push({ numbers[i], i });
            } else {
                while (true) {
                    int value = ss.top().first;
                    int index = ss.top().second;
                    answer[index] = numbers[i];
                    ss.pop();
                    if (ss.empty() || ss.top().first >= numbers[i]) {
                        ss.push({ numbers[i], i });
                        break;
                    }
                }
            }
        }
    }

    return answer;
}