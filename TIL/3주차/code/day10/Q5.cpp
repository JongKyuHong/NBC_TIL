#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <cmath>

using namespace std;

int main()
{
	ios::sync_with_stdio(false);
	cin.tie(NULL);

	string N;
	int B;
	cin >> N >> B;

	int jisu = 0;
	long long res = 0;

	for (int i = N.length() - 1; i >= 0; i--)
	{
		if (isalpha(N[i]))
		{
			int tmp = N[i] - 'A' + 10;
			int tmp2 = pow(B, jisu);
			res += tmp * tmp2;
		}
		else
		{
			int tmp2 = pow(B, jisu);
			res += (N[i] - '0') * tmp2;

		}
		
		jisu++;
	}

	cout << res << endl;


	return 0;
}