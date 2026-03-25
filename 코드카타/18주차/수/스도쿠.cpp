// https://www.acmicpc.net/problem/2580

#include <iostream>
#include <vector>

using namespace std;

vector<vector<int>> vec(9, vector<int>(9));
vector<pair<int, int>> coordinates;
bool found = false;

bool check(int r, int c, int target)
{
    // 같은 열, 행 체크
    for (int i = 0; i < 9; i++)
    {
        if (vec[r][i] == target || vec[i][c] == target) return false;    
    }
    
    int boxR = (r / 3) * 3;
    int boxC = (c / 3) * 3;
    for (int i = boxR; i < boxR + 3; i++)
    {
        for (int j = boxC; j < boxC + 3; j++)
        {
            if (vec[i][j] == target) return false;
        }
    }
    return true;
}

void solution(int index)
{
    if (found) return;
    if (index == (int)coordinates.size())
    {
        for (int i = 0; i < 9; i++)
        {
            for (int j = 0; j < 9; j++)
            {
                cout << vec[i][j] << " ";
            }
            cout << "\n";
        }
        found = true;
        return;
    }
    
    int r = coordinates[index].first;
    int c = coordinates[index].second;
    
    for (int i = 1; i <= 9; i++)
    {
        if (check(r, c, i))
        {
            vec[r][c] = i;
            solution(index+1);
            vec[r][c] = 0;
        }
    }
}

int main()
{
    ios_base::sync_with_stdio(false);
    cin.tie(0);

    for (int i = 0; i < 9; i++)
    {
        for (int j = 0; j < 9; j++)
        {
            int inp;
            cin >> inp;
            if (inp == 0)
            {
                coordinates.push_back({i, j});
            }
            vec[i][j] = inp;
        }
    }
    
    solution(0);
    
    return 0;
}