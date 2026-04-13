// https://www.acmicpc.net/problem/16395

#include <iostream>

using namespace std;

int main()
{
    ios_base::sync_with_stdio(false);
    cin.tie(0);

    int n, k;
    cin >> n >> k;
    
    int pascal[30][30];
    
    for (int i = 0; i < 30; i++)
    {
        pascal[i][0] = 1;
        pascal[i][i] = 1;
        for (int j = 1; j < i; j++)
        {
            pascal[i][j] = pascal[i-1][j-1] + pascal[i-1][j];
        }
    }
    
    cout << pascal[n-1][k-1];

    return 0;
}