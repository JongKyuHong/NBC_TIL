// https://www.acmicpc.net/problem/2447

#include <iostream>
#include <vector>

using namespace std;

void recursion(vector<vector<char>>& vec, int row, int col, int N)
{
    if (N == 3)
    {
        // cout << row << " " << col << "\n";
        // 여기서 실제로 그림
        for (int i = row; i < row+3; i++)
        {
            for (int j = col; j < col+3; j++)
            {
                if (i == row+1 && j == col+1) continue;
                vec[i][j] = '*';
            }
        }
        return;
    }
    
    recursion(vec, row, col, N/3);
    recursion(vec, row, col + N/3, N/3);
    recursion(vec, row, col + N*2/3, N/3);
    
    recursion(vec, row + N/3, col, N/3);
    // 중간 비우기
    //recursion(vec, row*2/3, col*2/3, N/3);
    recursion(vec, row + N/3, col + N*2/3, N/3);
    
    recursion(vec, row + N*2/3, col, N/3);
    recursion(vec, row + N*2/3, col + N/3, N/3);
    recursion(vec, row + N*2/3, col + N*2/3, N/3);
}


int main()
{
    ios_base::sync_with_stdio(false);
    cin.tie(0);
    
    int N;
    cin >> N;
    
    // 가운데 N/3 * N/3 정사각형
    vector<vector<char>> vec(N, vector<char>(N, ' '));
    // 항상기준은 왼쪽위
    recursion(vec, 0, 0, N);
    
    for (const auto& row : vec)
    {
        for (const auto& col : row)
        {
            cout << col;
        }
        cout << "\n";
    }
    
    
    return 0;
}