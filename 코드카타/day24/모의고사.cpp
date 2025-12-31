#include <string>
#include <vector>
#include <algorithm>

using namespace std;

vector<int> solution(vector<int> answers) {
    vector<int> answer;
    int person1[] = {1, 2, 3, 4, 5};
    int person2[] = {2,1,2,3,2,4,2,5};
    int person3[] = {3,3,1,1,2,2,4,4,5,5};
    int score1 = 0;
    int score2 = 0;
    int score3 = 0;
    
    for (int i = 0; i < answers.size(); i++){
        if (answers[i] == person1[i%5]){
            score1++;
        }
        if (answers[i] == person2[i%8]){
            score2++;
        }
        if (answers[i] == person3[i%10]){
            score3++;
        }
    }
    
    int maxV = max({score1, score2, score3});
    
    if (score1 == maxV){
        answer.push_back(1);
    }
    if (score2 == maxV){
        answer.push_back(2);
    }
    if (score3 == maxV){
        answer.push_back(3);
    }
    
    
    return answer;
}