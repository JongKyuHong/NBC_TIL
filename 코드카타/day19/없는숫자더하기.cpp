#include <string>
#include <vector>
#include <numeric>

using namespace std;

int solution(vector<int> numbers) {
    int answer = accumulate(numbers.begin(),numbers.end(), 0);
    return 45 - answer;
}