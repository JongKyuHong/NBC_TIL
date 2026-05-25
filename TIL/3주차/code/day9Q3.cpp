#include <string>
#include <iostream>

using namespace std;

class Student
{
private:
	string name;
	int age;

public:
	Student(string studentName, int studentAge) : name(studentName), age(studentAge) {};
	string getName() {
		return name;
	};
	int getAge() {
		return age;
	};
	string getInfo() {
		return "학생이름 : " + getName() +
			"\n" + "학생 나이 : " + to_string(getAge());
	};
};

class StudentPrinter
{
public:
	void print(Student& student)
	{
		cout << student.getInfo() << endl;
	}
};

int main()
{
	Student s1("hi", 23);

	StudentPrinter printer;

	printer.print(s1);
}