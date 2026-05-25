#include <iostream>
#include <algorithm>
#include <vector>
#include <queue>

using namespace std;

int main()
{
	vector<int> v = { 4, 1, 7, 3, 8, 5 };
	priority_queue<int> pq;

	for (int& x : v)
	{
		if (x >= 5)
		{
			pq.push(x);
		}
	}

	while (!pq.empty())
	{
		cout << pq.top() << " ";
		pq.pop();
	}
	cout << endl;
	return 0;
}

