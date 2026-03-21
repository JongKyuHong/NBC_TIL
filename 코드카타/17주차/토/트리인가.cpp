// https://www.acmicpc.net/problem/6416

#include <iostream>
#include <unordered_map>
#include <unordered_set>

using namespace std;

int main()
{
    ios_base::sync_with_stdio(false);
    cin.tie(0);

    unordered_set<int> CountNodeSet;
    int CountVertex = 0;
    unordered_map<int, int> um;
    int u, v, k = 1;
    int flag = 0;
    while (cin >> u >> v)
    {
        if (u == -1 && v == -1) break;
        if (u == 0 && v == 0)
        {
            // cout << CountNodeSet.size() << CountVertex << endl;
            // V - E = 1 만족하는지
            int nodeCount = (int)CountNodeSet.size();
            if (nodeCount == 0) cout << "Case " << k <<  " is a tree." << "\n";
            else
            {
                int rootCount = 0;
                for (const int CN : CountNodeSet)
                {
                    if (um.find(CN) == um.end()) rootCount++;
                }
                if (flag == 1 || rootCount != 1 || nodeCount-1 != CountVertex) cout << "Case " << k <<  " is not a tree." << "\n";
                else cout << "Case " << k <<  " is a tree." << "\n";
            }
            k++;
            flag = 0;
            CountVertex = 0;
            CountNodeSet.clear();
            um.clear();
            continue;
        }
        // 자기 자신에게 들어오는 길이 하나여야 함
        // 이미 존재하면 false
        if (um.find(v) != um.end())
        {
            flag = 1;
        } else
        {
            um[v] = u;
        }
        CountNodeSet.insert(u);
        CountNodeSet.insert(v);
        CountVertex++;
    }
    
    return 0;
}