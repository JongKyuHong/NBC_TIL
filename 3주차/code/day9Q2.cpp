#include <iostream>
#include <algorithm>
#include <vector>
#include <map>

using namespace std;

int main()
{
	vector<int> vec = { 10, 20, 30, 40, 50 };
	map<string, int> mp = {
		{"Alice", 90},
		{"Bob", 85},
		{"Charlie", 95}
	};

	cout << "벡터 정방향 출력" << "\n";
	for (auto it = vec.begin(); it != vec.end(); ++it)
	{
		cout << *it << " ";
	}
	cout << endl;

	cout << "벡터 역방향 출력" << "\n";
	for (auto it = vec.rbegin(); it != vec.rend(); ++it)
	{
		cout << *it << " ";
	}
	cout << "\n" << endl;

	cout << "map 정방향 출력" << "\n";
	for (auto it = mp.begin(); it != mp.end(); ++it)
	{
		cout << it->first << ": " << it->second << "\n";
	}
	cout << endl;

	cout << "map 역방향 출력" << "\n";
	for (auto it = mp.rbegin(); it != mp.rend(); ++it)
	{
		cout << it->first << ": " << it->second << "\n";
	}
	cout << endl;

	return 0;
}