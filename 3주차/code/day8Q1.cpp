#include <algorithm>
#include <iostream>

using namespace std;

class Battery
{
public:
	Battery(int initialCharge = 100)
	{
		charge = initialCharge;
	}

	int getCharge();
	void chargeBattery(int value);
	void useBattery(int value);

private:
	int charge = 0;
};

int Battery::getCharge()
{
	return charge;
}

void Battery::chargeBattery(int value = 7)
{
	charge += value;
}

void Battery::useBattery(int value = 5)
{
	charge -= value;
}

int main()
{
	Battery b;
	cout << "초기 배터리 잔량 : " << b.getCharge() << endl;

	b.useBattery();
	b.useBattery();
	cout << "배터리 사용 후 : " << b.getCharge() << endl;

	b.chargeBattery();
	cout << "배터리 충전 후 : " << b.getCharge() << endl;

	b.useBattery();
	cout << "배터리 잔량 : " << b.getCharge() << endl;

	return 0;
}