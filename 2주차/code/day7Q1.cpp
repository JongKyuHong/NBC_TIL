#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <set>
#include <stack>

using namespace std;

bool isNum(string& s)
{
	if (s.empty())
	{
		return false;
	}

	int index = 0;
	if (s[index] == '-' || isdigit(s[index]))
	{
		index++;
		bool flag = false;
		while (index < s.length())
		{
			if (isdigit(s[index]))
			{
				index++;
			}
			else if (s[index] == '.' && !flag)
			{
				index++;
				flag = true;
			}
			else
			{
				return false;
			}
		}
		return true;
	}
	else
	{
		return false;
	}
}

bool calculator(string& first, string& op, string& second, double& result)
{
	if (!(isNum(first) && isNum(second)))
	{
		return false;
	}

	double a = stod(first);
	double b = stod(second);

	if (op == "+")
	{
		result = a + b;
		return true;
	}
	else if (op == "-")
	{
		result = a - b;
		return true;
	}
	else if (op == "*")
	{
		result = a * b;
		return true;
	}
	else if (op == "/")
	{
		if (b == 0.0)
		{
			return false;
		}
		result = a / b;
		return true;
	}
	else
	{
		return false;
	}
}

int main()
{
	while (true)
	{
		cout << "사칙 연산을 입력해주세요." << "\n";
		cout << "예시) 1 + 1 = " << "\n";

		string inp;
		getline(cin, inp);

		istringstream ss(inp);
		vector<string> tokens;
		string token;

		while (ss >> token)
		{
			tokens.push_back(token);
		}

		if (tokens.size() != 4)
		{
			cout << "입력이 잘못 되었습니다." << "\n";
			continue;
		}

		string first = tokens[0];
		string op = tokens[1];
		string second = tokens[2];
		string equal = tokens[3];

		double result;
		if (!calculator(first, op, second, result))
		{
			cout << "입력이 잘못 되었습니다." << "\n";
			continue;
		}
		else
		{
			cout << first << " " << op << " " << second << " = " << result << endl;
		}
		cout << endl;
	}

	return 0;
}