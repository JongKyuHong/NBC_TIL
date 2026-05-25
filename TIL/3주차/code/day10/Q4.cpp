#include <iostream>
#include <string>
#include <sstream>
using namespace std;

int main()
{
	string inp;
	getline(cin, inp);

	istringstream ss(inp);
	string token;
	int cnt = 0;
	while (ss >> token)
	{
		cnt++;
	}


	cout << cnt << endl;

	return 0;
}