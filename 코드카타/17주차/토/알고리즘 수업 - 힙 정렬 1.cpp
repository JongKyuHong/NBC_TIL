// https://www.acmicpc.net/problem/24173

#include <iostream>
#include <vector>

using namespace std;

int N, K;
int Count = 0;
int first = -1;
int second = -1;

void heapify(vector<int>& vec, int size, int i)
{
    int smallest = i;
    int left = 2 * i + 1;
    int right = 2 * i + 2;
    
    if (left < size && vec[left] < vec[smallest])
    {
        smallest = left;
    }
    
    if (right < size && vec[right] < vec[smallest])
    {
        smallest = right;
    }
    
    if (smallest != i)
    {
        Count++;
        if (Count == K)
        {
            first = min(vec[i], vec[smallest]);
            second = max(vec[i], vec[smallest]);    
        }
        swap(vec[i], vec[smallest]);
        heapify(vec, size, smallest);
    }
}

void heapSort(vector<int>& vec)
{
    for (int i = N/2-1; i >= 0; i--)
    {
        heapify(vec, N, i);
    }
    
    for (int i = N-1; i > 0; i--)
    {
        Count++;
        if (Count == K)
        {
            first = min(vec[i], vec[0]);
            second = max(vec[i], vec[0]);    
        }
        swap(vec[0], vec[i]);
        heapify(vec, i, 0);
    }
}

int main()
{
    ios_base::sync_with_stdio(false);
    cin.tie(0);

    
    cin >> N >> K;
    vector<int> vec(N);
    for (int i = 0; i < N; i++)
    {
        cin >> vec[i];
    }
    heapSort(vec);
    
    // 정렬 후 
    if (first == -1)
    {
        cout << -1;
    } else
    {
        cout << first << " " << second;    
    }

    return 0;
}