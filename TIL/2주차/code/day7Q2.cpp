#include <iostream>
#include <stack>
#include <sstream>
#include <vector>
#include <stack>

using namespace std;

bool calculate(stack<double>& sstack, stack<string>& opstack)
{
	double first = sstack.top();
	sstack.pop();

	double second = sstack.top();
	sstack.pop();

	string op = opstack.top();
	opstack.pop();

	double result;
	if (op == "+")
	{
		result = first + second;
	}
	else if (op == "-")
	{
		result = first - second;
	}
	else if (op == "*")
	{
		result = first * second;
	}
	else if (op == "/")
	{
		if (second == 0.0)
		{
			return false;
		}
		result = first / second;
	}
	else
	{
		return false;
	}

	sstack.push(result);
	return true;
}

bool isOperator(const string& s)
{
	return s == "+" || s == "-" || s == "*" || s == "/";
}

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

int priorityOp(const string& s)
{
	if (s == "+" || s == "-")
	{
		return 1;
	}
	else if (s == "*" || s == "/")
	{
		return 2;
	}
	else
	{
		return 0;
	}

}

int main()
{
	while (true)
	{
		cout << "자동 계산기 사칙연산을 입려해주세요" << "\n";
		cout << "예시) 1 + 1 = " << "\n";
		cout << "q를 입력하면 종료됩니다." << "\n";

		cout << "> ";

		string inp;
		getline(cin, inp);

		if (inp[0] == 'q')
		{
			return 0;
		}

		istringstream ss(inp);
		vector<string> tokens;
		string token;

		while (ss >> token)
		{
			tokens.push_back(token);
		}

		stack<double> sstack;
		stack<string> opstack;

		for (auto& t : tokens)
		{
			if (isNum(t))
			{
				sstack.push(stod(t));
			}
			else if (isOperator(t))
			{
				while (!opstack.empty() && priorityOp(opstack.top()) > priorityOp(t))
				{
					if (!calculate(sstack, opstack))
					{
						cout << "입력이 잘못 되었습니다." << "\n";
						goto next_inp;
					}
				}
				opstack.push(t);
			}
			else if (t == "(")
			{
				opstack.push(t);
			}
			else if (t == ")")
			{
				while (!opstack.empty() && opstack.top() != "(")
				{
					if (!calculate(sstack, opstack))
					{
						cout << "입력이 잘못 되었습니다" << "\n";
						goto next_inp;
					}
				}
				if (opstack.empty())
				{
					cout << "입력이 잘못 되었습니다" << "\n";
					goto next_inp;
				}
				opstack.pop();
			}
			else
			{
				cout << "입력이 잘못되었습니다." << "\n";
			}
		}
	next_inp:
		;
	}

	return 0;
}