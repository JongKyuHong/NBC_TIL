#include <iostream>
#include <string>
#include <unordered_set>

using namespace std;

bool solution(string& Inp)
{
	unordered_set<char> Words;
	int Index = 0;
	char current = '1';
	while (Index < Inp.length())
	{
		if (current != Inp[Index])
		{
			if (Words.find(Inp[Index]) != Words.end())
			{
				return false;
			}
			current = Inp[Index];
			Words.insert(current);
		}
		Index++;
	}
	return true;
}


int main()
{
	// 각문자가 연속해서 나와야함 한번 나온 문자는 연속되어서 나오는거 아니면 다시 나오면 안됨
	// 가장 확실한거는 unordered_set이 아닐까? 들어있으면
	int N;
	cin >> N;

	int result = 0;

	for (int i = 0; i < N; i++)
	{
		string Inp;
		cin >> Inp;

		if (solution(Inp))
		{
			result++;
		}
	}

	cout << result << endl;
	return 0;
}