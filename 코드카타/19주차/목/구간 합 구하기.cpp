// https://www.acmicpc.net/problem/2042

#include <iostream>
#include <vector>

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
    return tree[node] = build(node*2, start, mid) + build(node*2+1, mid+1, end);
}

void update(int node, int start, int end, int idx, long long value)
{
    if (start == end)
    {
        tree[node] = value;
        return;
    }
    int mid = (start + end) / 2;
    if (idx <= mid)
    {
        update(node*2, start, mid, idx, value);
    } else
    {
        update(node*2+1, mid+1, end, idx, value);
    }
    tree[node] = tree[node*2] + tree[node*2+1];
}

long long query(int node, int start, int end, int left, int right)
{
    if (right < start || end < left) return 0;
    if (left <= start && end <= right) return tree[node];
    int mid = (start + end) / 2;
    return query(node*2, start, mid, left, right) + query(node*2 + 1, mid +1, end, left, right);
}

int main()
{
    ios_base::sync_with_stdio(false);
    cin.tie(0);
    
    int N, M, K;
    cin >> N >> M >> K;
    nums.resize(N+1);
    tree.resize(4*N+1);
    
    for (int i = 1; i <= N; i++)
    {
        cin >> nums[i];
    }
    build(1, 1, N);
    
    for (int i = 0; i < M+K; i++)
    {
        long long a, b, c;
        cin >> a >> b >> c;
        if (a == 1)
        {
            update(1, 1, N, (int)b, c);
        } else
        {
            cout << query(1, 1, N, (int)b, (int)c) << "\n";             
        }
    }

    return 0;
}