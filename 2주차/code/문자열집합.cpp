#include <iostream>
#include <string>
#include <unordered_set>

using namespace std;

int main()
{
	ios::sync_with_stdio(false);
	cin.tie(nullptr);

	int N, M;
	cin >> N >> M;

	unordered_set<string> us;
	int result = 0;

	for (int i = 0; i < N; i++)
	{
		string inp;
		cin >> inp;
		us.insert(inp);
	}

	for (int i = 0; i < M; i++)
	{
		string inp;
		cin >> inp;
		
		if (us.find(inp) != us.end())
		{
			result++;
		}
	}
	
	cout << result << endl;
	return 0;
}