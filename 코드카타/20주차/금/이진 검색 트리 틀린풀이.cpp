// https://www.acmicpc.net/problem/1539
// 시간초과
#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

struct Node
{
    int node;
    int left = -1;
    int right = -1;
    
    Node(int val) : node(val) {}
    Node() : node(0) {}
};

long long total = 0;

void insert(vector<Node>& tree, int root, vector<int>& P, int index)
{
    if (tree[root].node > P[index])
    {
        if (tree[root].left == -1)
        {
            tree[root].left = index;
        } else
        {
            insert(tree, tree[root].left, P, index);
        }
    } else
    {
        if (tree[root].right == -1)
        {
            tree[root].right = index;
        } else
        {
            insert(tree, tree[root].right, P, index);
        }
    }
}

void dfs(vector<Node>& tree, int index, long long depth)
{
    total += depth;
    if (tree[index].left != -1)
    {
        dfs(tree, tree[index].left, depth+1);
    }
    if (tree[index].right != -1)
    {
        dfs(tree, tree[index].right, depth+1);
    }
}

int main()
{
    ios_base::sync_with_stdio(false);
    cin.tie(0);
    
    int N;
    cin >> N;
    vector<int> P(N);
    for (int i= 0; i < N; i++)
    {
        cin >> P[i];
    }
    
    vector<Node> tree(N);
    
    tree[0].node = P[0];
    
    for (int i = 1; i < N; i++)
    {
        tree[i].node = P[i];
        insert(tree, 0, P, i);
    }
    
    dfs(tree, 0, 1);
    
    cout << total;
    return 0;
}