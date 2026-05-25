#include <iostream>
#include <string>
#include <unordered_map>
using namespace std;

int main()
{
	ios::sync_with_stdio(false);
	cin.tie(nullptr);

	int N;
	cin >> N;
	unordered_map<int, int> Cards;
	for (int i = 0; i < N; i++)
	{
		int Inp;
		cin >> Inp;
		Cards.insert({ Inp, 1 });
	}

	int M;
	cin >> M;
	for (int i = 0; i < M; i++)
	{
		int Inp;
		cin >> Inp;
		if (Cards.find(Inp) != Cards.end())
		{
			cout << "1 ";
		}
		else
		{
			cout << "0 ";
		}
	}

	cout << endl;

	return 0;
}