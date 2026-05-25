#include <iostream>
#include <string>
#include <unordered_map>

using namespace std;

int main()
{
	double TotalScore = 0;
	double SumValue = 0;
	unordered_map<string, double> ScoreBoard = {
		{"A+", 4.5},
		{"A0", 4},
		{"B+", 3.5},
		{"B0", 3},
		{"C+", 2.5},
		{"C0", 2},
		{"D+", 1.5},
		{"D0", 1},
		{"F", 0},
	};

	
	for (int i = 0; i < 20; i++)
	{
		string subject, grade;
		double score;

		cin >> subject >> score >> grade;

		if (grade == "P")
		{
			continue;
		}
		
		SumValue += score * ScoreBoard[grade];
		TotalScore += score;
	}

	double result = SumValue / TotalScore;
	
	cout << fixed;
	cout.precision(6);
	cout << result << endl;
	

	return 0;
}