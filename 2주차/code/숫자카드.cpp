#include <iostream>
#include <string>
#include <map>
using namespace std;

int main()
{
	int N;
	cin >> N;
	map<int, int> Cards;
	for (int i = 0; i < N; i++)
	{
		int Inp;
		cin >> Inp;
		Cards.insert({ Inp, 1 });
	}

	int M;
	cin >> M;
	string result;
	for (int i = 0; i < M; i++)
	{
		int Inp;
		cin >> Inp;
		if (Cards.find(Inp) != Cards.end())
		{
			result += "1 ";
		}
		else
		{
			result += "0 ";
		}
	}

	result.erase(result.length() - 1);

	cout << result << endl;

	return 0;
}