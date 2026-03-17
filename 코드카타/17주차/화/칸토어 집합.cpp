// https://www.acmicpc.net/problem/4779

#include <iostream>
#include <vector>
#include <cmath>

using namespace std;

void find(int l, int r, vector<int> &vec)
{
    if (l == r)
    {
        vec[l] = 1;
        return;
    }
    // 전체 길이 이를 3등분
    int length = (r-l+1)/3; 
    // 처음 부분
    find (l, l+length-1, vec);
    find (l+length*2, r, vec);
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(NULL);
    int N;
    while (cin >> N)
    {   
        int value = (int)pow(3, N);
        vector<int> vec(value, 0);
        // 분할
        find(0, value-1, vec);
        
        string tmp = "";
        for (int i = 0; i < value; i++)
        {
            tmp += (vec[i] == 1 ? "-" : " ");
        }
        cout << tmp << "\n";
    }
    
    return 0;
}