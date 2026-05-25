#include <iostream>
#include <string>
#include <map>

using namespace std;

void print(const int& target)
{
	if (target % 3 == 0 && target % 5 == 0)
	{
		cout << "FizzBuzz" << endl;
	}
	else if (target % 3 == 0 && target % 5 != 0)
	{
		cout << "Fizz" << endl;
	}
	else if (target % 3 != 0 && target % 5 == 0)
	{
		cout << "Buzz" << endl;
	}
	else
	{
		cout << target << endl;
	}
}

int main()
{
	string inp1, inp2, inp3;

	cin >> inp1;
	cin >> inp2;
	cin >> inp3;

	if (isdigit(inp1[0]))
	{
		int target = stoi(inp1) + 3;
		print(target);
	}
	else if (isdigit(inp2[0]))
	{
		int target = stoi(inp2) + 2;
		print(target);
	}
	else if (isdigit(inp3[0]))
	{
		int target = stoi(inp3) + 1;
		print(target);
	}
	else {
		// 만약에 세개 다 숫자가 아니면 직접 찾아야 한다.
		map<string, int> mm = {
			{"FizzBuzz" , 0},
			{"Fizz" , 1},
			{"Buzz" , 2},
		};

		int arr[] = { mm[inp1], mm[inp2], mm[inp3] };

		int arrIndex = 0;
		int start = 1;

		while (true)
		{
			if (arr[arrIndex] == 0)
			{
				if (start % 3 == 0 && start % 5 == 0)
				{
					arrIndex++;
				}
				else
				{
					arrIndex = 0;
				}
			}
			else if (arr[arrIndex] == 1)
			{
				if (start % 3 == 0 && start % 5 != 0)
				{
					arrIndex++;
				}
				else
				{
					arrIndex = 0;
				}
			}
			else
			{
				if (start % 3 != 0 && start % 5 == 0)
				{
					arrIndex++;
				}
				else
				{
					arrIndex = 0;
				}
			}
			if (arrIndex == 3)
			{
				break;
			}
			start++;
		}

		print(start);
	}

	return 0;
}