#include <iostream>
#include <vector>


using namespace std;

int main()
{
	cin.tie(NULL);
	ios::sync_with_stdio(false);

	vector<int> vec(26, -1);

	string S;
	cin >> S;

	for (int i = 0; i < S.length(); i++)
	{
		int index = S[i] - 'a';

		if (vec[index] == -1)
		{
			vec[index] = i;
		}
	}
	
	for (int& v : vec)
	{
		cout << v << " ";
	}
	cout << endl;


	return 0;
}