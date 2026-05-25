#include <iostream>
#include <string>

using namespace std;

int main()
{
	string Inp;
	cin >> Inp;

	int N = Inp.length();
	int result = 0;
	int Idx = 0;
	
	while (Idx < N)
	{
		if (Inp[Idx] == 'c')
		{
			if (Idx + 1 < N && (Inp[Idx+1] == '-' || Inp[Idx+1] == '='))
			{
				
				Idx++;
			}
		} 
		else if (Inp[Idx] == 'd')
		{
			if (Idx + 1 < N && Inp[Idx+1] == 'z')
			{
				if (Idx + 2 < N && Inp[Idx + 2] == '=')
				{
					Idx += 2;
				}
			}
			else if(Idx+1 < N && Inp[Idx+1] == '-')
			{
				
				Idx++;
			}
		}
		else if (Inp[Idx] == 'l')
		{
			if (Idx + 1 < N && Inp[Idx + 1] == 'j')
			{
				
				Idx++;
			}
		}
		else if (Inp[Idx] == 'n')
		{
			if (Idx + 1 < N && Inp[Idx + 1] == 'j')
			{
				
				Idx++;
			}
		}
		else if (Inp[Idx] == 's')
		{
			if (Idx + 1 < N && Inp[Idx + 1] == '=')
			{
				
				Idx++;
			}
		}
		else if (Inp[Idx] == 'z')
		{
			if (Idx + 1 < N && Inp[Idx + 1] == '=')
			{
				
				Idx++;
			}
		}

		result++;
		Idx++;
	}

	cout << result << endl;

	return 0;
}