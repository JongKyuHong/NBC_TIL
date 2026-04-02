// https://www.acmicpc.net/problem/10868

#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

vector<long long> nums;
vector<long long> tree;

long long build(int node, int start, int end)
{
    if (start == end)
    {
        return tree[node] = nums[start];
    }    
    int mid = (start+end)/2;
    return tree[node] = min(build(node*2, start, mid), build(node*2+1, mid+1, end));
}

long long query(int node, int start, int end, int left, int right)
{
    if (left > end || right < start) return 10e9;
    if (start >= left && right >= end) return tree[node];
    int mid = (start + end) / 2;
    return min(query(node*2, start, mid, left, right), query(node*2+1, mid+1, end, left, right));
}

int main()
{
    ios_base::sync_with_stdio(false);
    cin.tie(0);
    
    int N, M;
    cin >> N >> M;
    
    nums.resize(N+1);
    tree.resize(4*N+1);
    
    for (int i = 1; i <= N; i++)
    {
        cin >> nums[i];
    }
    build(1, 1, N);

    for (int i = 0; i < M; i++)
    {
        int a, b;
        cin >> a >> b;
        cout << query(1, 1, N, a, b) << "\n";
    }
    
    return 0;
}