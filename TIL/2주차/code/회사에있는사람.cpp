#include <iostream>
#include <string>
#include <unordered_set>
#include <algorithm>

using namespace std;

int main()
{
	ios::sync_with_stdio(false);
	cin.tie(nullptr);

	int n;
	cin >> n;

	unordered_set<string> people;

	for (int i = 0; i < n; i++)
	{
		string name, state;
		cin >> name >> state;

		if (state == "enter")
		{
			people.insert(name);
		}
		else
		{
			people.erase(name);
		}

	}

	vector<string> v(people.begin(), people.end());

	sort(v.begin(), v.end(), [](auto& a, auto& b) {
		return a > b;
		});

	for (auto& p : v)
	{
		cout << p << "\n";
	}

	return 0;
}