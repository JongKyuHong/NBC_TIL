#include <iostream>
#include <vector>
using namespace std;

class Adventure
{
public:
	virtual void useSkill() = 0;
	virtual ~Adventure() {}
};

class Warrior : public Adventure
{
public:
	void useSkill() {
		cout << "Warrior uses Slash!" << endl;
	}
};

class Mage : public Adventure 
{
public:
	void useSkill() {
		cout << "Mage casts Fireball!" << endl;
	}
};

class Archer : public Adventure
{
public:
	void useSkill() {
		cout << "Archer shoots an Arrow!" << endl;
	}
};

int main()
{
	vector<Adventure*> adventures;
	adventures.push_back(new Warrior());
	adventures.push_back(new Mage());
	adventures.push_back(new Archer());

	for (size_t i = 0; i < adventures.size(); i++)
	{
		adventures[i]->useSkill();
	}

	for (size_t i = 0; i < adventures.size(); i++)
	{
		delete adventures[i];
	}

	return 0;
}