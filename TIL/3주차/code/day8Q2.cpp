#include <algorithm>
#include <iostream>

using namespace std;

class Fraction
{
public:
	Fraction() {
		this->numerator = 0;
		this->denominator = 1;
	}
	Fraction(int numerator, int denominator)
	{
		this->numerator = numerator;
		this->denominator = denominator;
	}

	int gcd(int a, int b)
	{
		while (b != 0)
		{
			int tmp = b;
			b = a % b;
			a = tmp;
		}
		return a;
	}
	void simplify() 
	{
		int initialValue = gcd(numerator, denominator);
		this->numerator = numerator / initialValue;
		this->denominator = denominator / initialValue;
	}
	Fraction multiply(const Fraction& other)
	{
		int newNumerator = numerator * other.numerator;
		int newDenominator = denominator * other.denominator;

		Fraction result(newNumerator, newDenominator);

		result.simplify();

		return result;
	}
	void display()
	{
		cout << numerator << "/" << denominator << endl;
	}

private:
	int numerator;
	int denominator;
};

int main()
{
	Fraction f1(1, 2);
	Fraction f2(3, 4);

	Fraction result = f1.multiply(f2);

	cout << "곱한 결과 : ";
	result.display();
	cout << endl;

	return 0;
}