#include <iostream>
#include <string>
#include <set>
using namespace std;

int main()
{
	string Inp;
	cin >> Inp;

	int N = Inp.length();
	set<string> Words;

	for (int i = 0; i < N; i++)
	{
		for (int j = i; j < N; j++)
		{
			Words.insert(Inp.substr(i, j-i+1));
		}
	}

	cout << Words.size() << endl;

	return 0;
}