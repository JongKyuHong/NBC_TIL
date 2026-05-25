#include <iostream>
#include <string>

using namespace std;

int main()
{
	cin.tie(NULL);
	ios::sync_with_stdio(false);

	string keyword;
	cin >> keyword;

	int N;
	cin >> N;

	int result = 0;

	for (int i = 0; i < N; i++)
	{
		string ring;
		cin >> ring;
		ring = ring + ring;
		if (ring.find(keyword) != string::npos)
		{
			result++;
		}
	}

	cout << result << endl;


	return 0;
}