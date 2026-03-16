// https://www.acmicpc.net/problem/24060

#include <iostream>
#include <vector>

using namespace std;
int N, K;
int cnt = 0;
int ans = -1;

void merge(vector<int>& A, int p, int q, int r) {
    int i = p, j = q + 1, t = 0;
    vector<int> tmp;

    while (i <= q && j <= r) {
        if (A[i] <= A[j]) tmp.push_back(A[i++]);
        else tmp.push_back(A[j++]);
        cnt++;
        if (cnt == K) ans = tmp.back();
    }

    while (i <= q) {
        tmp.push_back(A[i++]);
        cnt++;
        if (cnt == K) ans = tmp.back();
    }

    while (j <= r) {
        tmp.push_back(A[j++]);
        cnt++;
        if (cnt == K) ans = tmp.back();
    }

    for (int k = 0; k < (int)tmp.size(); k++)
        A[p + k] = tmp[k];
}

void merge_sort(vector<int>& A, int p, int r) {
    if (p < r) {
        int q = (p + r) / 2;
        merge_sort(A, p, q);
        merge_sort(A, q + 1, r);
        merge(A, p, q, r);
    }
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(NULL);

    cin >> N >> K;
    vector<int> A;
    for (int i = 0; i < N; i++)
    {
        int tmp;
        cin >> tmp;
        A.push_back(tmp);
    }
    
    merge_sort(A, 0, N-1);

    cout << ans;
    return 0;
}