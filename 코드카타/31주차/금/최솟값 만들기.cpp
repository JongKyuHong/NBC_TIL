// https://school.programmers.co.kr/learn/courses/30/lessons/12941


#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

int solution(vector<int> A, vector<int> B)
{
    sort(A.begin(), A.end());
    sort(B.begin(), B.end(), greater<int>());
    
    int len = A.size();
    int answer = 0;
    for (int i = 0; i < len; i++){
        answer += A[i] * B[i];
    }
    return answer;
}