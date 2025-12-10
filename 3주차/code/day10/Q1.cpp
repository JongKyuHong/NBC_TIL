#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

int main()
{
	cin.tie(NULL);
	ios::sync_with_stdio(false);

	int t;
	cin >> t;

	vector<long long> vec(68, 1);

	vec[2] = 2;
	vec[3] = 4;

	for (int i = 4; i <= 67; i++)
	{
		vec[i] = vec[i - 1] + vec[i - 2] + vec[i - 3] + vec[i - 4];
	}

	for (int i = 0; i < t; i++)
	{
		int n;
		cin >> n;

		cout << vec[n] << "\n";
	}

	cout << endl;

	return 0;
}