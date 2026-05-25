#include <iostream>
#include <string>
#include <unordered_map>
#include <unordered_set>
using namespace std;

int main()
{
	int A, B;
	cin >> A >> B;

	unordered_map<int, int> AMap;
	unordered_map<int, int> BMap;

	for (int i = 0; i < A; i++)
	{
		int Inp;
		cin >> Inp;
		AMap.insert({ Inp, 1 });
	}

	for (int i = 0; i < B; i++)
	{
		int Inp;
		cin >> Inp;
		BMap.insert({ Inp, 1 });
	}

	unordered_set<int> Result;

	for (auto& M : AMap)
	{
		if (BMap.find(M.first) == BMap.end())
		{
			Result.insert(M.first);
		}
	}

	for (auto& M : BMap)
	{
		if (AMap.find(M.first) == AMap.end())
		{
			Result.insert(M.first);
		}
	}

	cout << Result.size() << endl;

	return 0;
}