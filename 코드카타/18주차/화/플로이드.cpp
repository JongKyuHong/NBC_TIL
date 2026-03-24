// https://www.acmicpc.net/problem/11404

#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

const int MAX_VALUE = 1e9;

int main()
{
    ios_base::sync_with_stdio(false);
    cin.tie(0);

    int n, m;
    cin >> n;
    cin >> m; 
    
    vector<vector<int>> vec(n, vector<int>(n, MAX_VALUE));
    for (int i = 0; i < m; i++)
    {
        int a, b, c;
        cin >> a >> b >> c;
        a--; b--;
        vec[a][b] = min(vec[a][b], c);
    }
    
    for (int i = 0; i < n; i++)
    {
        vec[i][i] = 0;
    }
    
    for (int k = 0; k < n; k++)
    {
        for (int i = 0; i < n; i++)
        {
            for (int j = 0; j < n; j++)
            {
                vec[i][j] = min(vec[i][j], vec[i][k] + vec[k][j]);
            }
        }
    }
    
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            if (vec[i][j] == MAX_VALUE)
            {
                cout << 0 << " ";
            } else
            {
                cout << vec[i][j] << " ";
            }
        }
        cout << "\n";
    }
    
    return 0;
}