#include <iostream>
#include <algorithm>
#include <vector>

using namespace std;

int main()
{
	vector<int> v = { 15, 3, 9, 8, 5, 2, 10, 7, 6 };
	vector<int> v2;

	for (auto& v1 : v)
	{
		if (v1 % 2 == 0) {
			v2.push_back(v1);
		}
	}
	
	nth_element(v2.begin(), v2.begin() + v2.size() - 3, v2.end());
	
	cout << v2[v2.size() - 3] << endl;

	return 0;
}
