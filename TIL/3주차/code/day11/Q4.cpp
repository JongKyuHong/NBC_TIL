#include <iostream>

using namespace std;

class Thermometer
{
private:
	double temperature;
	bool PrintType;
public:
	Thermometer() : temperature(0.0), PrintType(true) {};
	double GetTemperature() const
	{
		return temperature;
	}
	bool SetTemperature(const double& input)
	{
		if (input > 100 || input < -100)
		{
			return false;
		}
		temperature = input;
		return true;
	}
	void SetType()
	{
		PrintType = !PrintType;
		if (PrintType)
		{
			cout << "℃ 전환 완료\n";
		}
		else
		{
			cout << "℉ 전환 완료\n";
		}
	}
	void DisplayTemp()
	{
		if (PrintType)
		{
			cout << "현재 온도 : " << temperature << "℃";
		}
		else
		{
			double tmp = temperature * 1.8 + 32;
			cout << "현재 온도 : " << tmp << "℉";
		}
		cout << " 입니다.\n";
	}
};

void Display()
{
	cout << "\n[-------------------------------]\
		\n1) 온도 설정\
		\n2) 표시 모드 전환\
		\n3) 온도 표시\n";
}


int main()
{
	cout << fixed;
	cout.precision(4);
	Thermometer Th;
	while (true)
	{
		Display();
		string inp;
		cin >> inp;
		if (inp == "1")
		{
			cout << "설정할 온도를 입력해주세요 : ";
			double value;
			cin >> value;
			if (!Th.SetTemperature(value))
			{
				cout << "온도입력에 실패했습니다 -100~100도사이만 가능\n";
			}
		} 
		else if (inp == "2")
		{
			Th.SetType();
		}
		else if (inp == "3")
		{
			double tmp = Th.GetTemperature();
			Th.DisplayTemp();
		}
		else
		{

		}
	}

	return 0;
}