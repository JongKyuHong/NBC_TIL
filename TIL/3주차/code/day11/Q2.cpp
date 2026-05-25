#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

using namespace std;

bool compare(string& a, string& b)
{
	if (a.length() != b.length())
	{
		return a.length() < b.length();
	}
	return a < b;
}

int main()
{
	int N;
	cin >> N;

	vector<string> vec;

	for (int i = 0; i < N; i++)
	{
		string inp;
		cin >> inp;
		vec.push_back(inp);
	}
	sort(vec.begin(), vec.end(), compare);
	vec.erase(unique(vec.begin(), vec.end()), vec.end());
	for (string& v : vec)
	{
		cout << v << "\n";
	}
	return 0;
}