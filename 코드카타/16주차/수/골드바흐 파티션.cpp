// https://www.acmicpc.net/problem/17103

#include <iostream>
#include <vector>

using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(NULL);

    int T;
    cin >> T;
    int maxValue = 1000001;
    vector<bool> isPrime(maxValue, true);
    isPrime[0] = isPrime[1] = false;
    for (int i = 2; (long long) i * i <= maxValue; i++)
    {
        if (isPrime[i])
        {
            for (int j = i * i; j <= maxValue; j += i)
            {
                isPrime[j] = false;
            }
        }
    }
    vector<int> Prime;
    for (int i = 2; i <= maxValue; i++)
        if (isPrime[i]) Prime.push_back(i);
    
    
    for (int t = 0; t < T; t++)
    {
        int N;
        cin >> N;
        
        int start = 0;
        int end = Prime.size() - 1;
        int count = 0;
        
        while (end >= 0 && Prime[end] > N) end--;
        
        while (start <= end)
        {
            int sumV = Prime[start] + Prime[end];
            
            if (sumV == N)
            {
                count++;
                start++;
                end--;
            } else if (sumV < N)
            {
                start++;
            } else
            {
                end--;
            }
        }
        
        cout << count << "\n";
    }
    
    return 0;
}