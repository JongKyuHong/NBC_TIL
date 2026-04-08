// https://www.acmicpc.net/problem/5639

#include <iostream>
#include <vector>

using namespace std;

void makePostOrder(int start, int end, vector<int>& tree)
{
    if (start >= end) return;
    int root = tree[start];
    int rightStart = start+1;
    
    while (rightStart < end && tree[rightStart] < root)
    {
        rightStart++;
    }
    
    makePostOrder(start+1, rightStart, tree);
    makePostOrder(rightStart, end, tree);
    cout << root << "\n";
}

int main()
{
    ios_base::sync_with_stdio(false);
    cin.tie(0);
    
    // 전위 순회를 후위순회로 변경하기
    vector<int> tree;

    int inp;
    
    while (cin >> inp)
    {
        tree.push_back(inp);
    }
    
    int root = tree[0];
    makePostOrder(0, (int)tree.size(), tree);
    
    return 0;
}