#include <iostream>
#include <algorithm>
#include <vector>

using namespace std;



int main()
{
	vector<int> v = { 1, 2, 3, 4 };
	vector<int> v2 = { 2, 3, 4 };

	do {
		cout << 1 << " " << v2[0] << " " << v2[1] << endl;
	} while (next_permutation(v2.begin(), v2.end()));
}