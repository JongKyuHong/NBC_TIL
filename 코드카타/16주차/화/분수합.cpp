// https://www.acmicpc.net/problem/1735

#include <iostream>
#include <numeric>

using namespace std;

int gcd(int a, int b)
{
    while (b != 0)
    {
        a = a % b;
        swap(a, b);
    }
    return a;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(NULL);
    
    int A, B, C, D;
    cin >> A >> B;
    cin >> C >> D;

    int child = A*D + C*B;
    int parent = B*D;
    int gcdValue = gcd(child,parent);
    child /= gcdValue;
    parent /= gcdValue;
    cout << child << " " << parent << "\n";
}