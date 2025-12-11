#include <iostream>

using namespace std;

class Car
{
public:
	Car(const string& Name) : Name(Name) {};
	void Accelerate() 
	{
		Speed += 10.0;
	}
	void Break()
	{
		if (Speed >= 10)
		{
			Speed -= 10.0;
		}
	}
	void DisplayInfo()
	{
		cout << "자동차 이름 : " << Name << "\n" << "현재 속도 : " << Speed << endl;
	}

private:
	string Name;
	double Speed = 0;
};

void Display()
{
	cout << "[-------------------]\n" << "1) 가속\n" << "2) 브레이크\n" << "3) 종료\n";
}

int main()
{
	Car car("붕붕이");
	while (true)
	{
		Display();
		car.DisplayInfo();
		string inp;
		cin >> inp;
		if (inp == "1")
		{
			car.Accelerate();
		}
		else if (inp == "2")
		{
			car.Break();
		}
		else if (inp == "3")
		{
			break;
		}
		else 
		{
			cout << "입력이 잘못되었습니다.\n";
		}
	}

	return 0;
}