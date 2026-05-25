#include <iostream>
#include <vector>

using namespace std;

class Phone {
public:
	virtual void displayBrand() = 0;
	virtual void showFeature() = 0;
	virtual ~Phone() {};
};

class Samsung : public Phone {
public:
	void displayBrand() {
		cout << "Samsung" << endl;
	}

	void showFeature() {
		cout << "Galaxy S 시리즈" << endl;
	}
};

class Apple : public Phone {
public:
	void displayBrand() {
		cout << "Apple" << endl;
	}
	void showFeature() {
		cout << "iPhone Pro 시리즈" << endl;
	}
};

class Xiaomi : public Phone {
public:
	void displayBrand() {
		cout << "Xiaomi" << endl;
	}
	void showFeature() {
		cout << "Redmi Note 시리즈" << endl;
	}
};

int main() {
	vector<Phone*> CellPhones;

	CellPhones.push_back(new Samsung());
	CellPhones.push_back(new Apple());
	CellPhones.push_back(new Xiaomi());

	for (size_t i = 0; i < CellPhones.size(); i++)
	{
		CellPhones[i]->displayBrand();
		CellPhones[i]->showFeature();
	}

	for (size_t i = 0; i < CellPhones.size(); i++)
	{
		delete CellPhones[i];
	}

	return 0;
}