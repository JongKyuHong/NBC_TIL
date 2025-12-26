#include <string>
#include <vector>
#include <algorithm>

using namespace std;

int solution(vector<vector<int>> sizes) {
    int answer = 0;
    int maxVmax = 0;
    int minVmax = 0;

    for (int i = 0; i < sizes.size(); i++) {
        maxVmax = max(maxVmax, max(sizes[i][0], sizes[i][1]));
        minVmax = max(minVmax, min(sizes[i][0], sizes[i][1]));
    }

    return maxVmax * minVmax;
}