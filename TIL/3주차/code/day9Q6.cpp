#include <iostream>
#include <string>

using namespace std;

enum { HP, MP, ATK, DEF };

void SetHPMP(int* statusPtr)
{
	while (true)
	{
		cout << "HP와 MP를 입력해주세요 : ";
		int InpHP;
		int InpMP;
		cin >> InpHP >> InpMP;

		if (InpHP <= 50 || InpMP <= 50)
		{
			cout << "HP나 MP의 값이 너무 작습니다. 다시 입력해주세요.\n";
			continue;
		}
		else
		{
			statusPtr[HP] = InpHP;
			statusPtr[MP] = InpMP;
			return;
		}
	}
}

void SetAtkDef(int* statusPtr)
{
	while (true)
	{
		cout << "공격력과 방어력을 입력해주세요 : ";
		int InpAtk;
		int InpDef;

		cin >> InpAtk >> InpDef;

		if (InpAtk <= 0 || InpDef <= 0)
		{
			cout << "공격력이나 방어력이 너무 작습니다. 다시 입력해주세요.\n";
		}
		else
		{
			statusPtr[2] = InpAtk;
			statusPtr[3] = InpDef;
			return;
		}
	}
}

void setPotion(const int& count, int* p_HPPotion, int* p_MPPotion)
{
	*p_HPPotion = count;
	*p_MPPotion = count;
}

// 시스템 메시지를 출력하는 함수, 항상 메뉴가 나오도록 했다.
void printDisplay(const bool& IsItFirst)
{
	cout << "<스탯 관리 시스템>\n" <<
			"1. HP UP\n2. MP UP\n3. 공격력 UP\n4. 방어력 UP\n5. 현재 능력치\n6. Level UP\n0. 나가기\n";
	cout << "번호를 선택해주세요: ";
}

// 포션을 가지고있는지 확인하는 함수
bool HasPotion(int* Potion)
{
	if (*Potion <= 0)
	{
		return false;
	}
	return true;
}

// 포션이 없는경우 상태 메시지를 출력하는 함수
void NotifyPotionEmpty()
{
	cout << "포션이 부족합니다." << endl;
}

// void SetHpUp(const int& value, int* statusPtr, int* Potion)
// {
// 	*statusPtr += value;
// 	*Potion -= 1;
// }

// 포션을 1개 차감하는 함수
void SetHPMpUp(const int& value, int* statusPtr, int* Potion)
{
	*statusPtr += value;
	*Potion -= 1;
}

// 공격력/방어력을 늘려주는 함수
void SetAtkDefUp(const int& value, int* statusPtr)
{
	*statusPtr = value * (*statusPtr);
}

// void SetDefUp(const int& value, int* statusPtr)
// {
// 	*statusPtr = value * (*statusPtr);
// }

// HP/MP 변경 메시지 출력
void PrintHPMPChange(const int& value, const string& StName,  int& CurrentSt, int& CurrentPotionCnt)
{
	cout << "*" << StName << "가" << value << "증가되었습니다." << "포션이 1개 차감됩니다.\n" 
		<< "현재 " << StName << ": " << CurrentSt << "\n"
		<< "남은 포션 수 : " << CurrentPotionCnt << endl;
}

// 공격력/방어력 변경 메시지 출력
void PrintATKDEFChange(const int& Multiply, const string& StName,  int& CurrentSt)
{
	cout << "*" << StName << "이 " << Multiply << "배로 증가되었습니다.\n" <<
		"현재 " << StName << ": " << CurrentSt << endl;
}

// 현재 스텟 출력 함수
void printSt(int* statusPtr) 
{
	cout << "* HP : " << statusPtr[0] << ", MP : " << statusPtr[1] << ", 공격력 : " << statusPtr[2] << ", 방어력 : " << statusPtr[3] << endl;
}

// 레벨업 함수
// HP, MP포션을 하나씩 더 지급
void LevelUp(int* HpPotion, int*MpPotion)
{
	*HpPotion += 1;
	*MpPotion += 1;
}

// 레벨업시 메시지 출력 함수
// 현재 포션수를 출력함
void LevelUpPrint(const int& HpPotion, const int& MpPotion)
{
	cout << "* 레벨업! HP/MP 포션이 지급됩니다.\n" <<
		"남은 HP/MP 포션 수 : " << HpPotion << "/" << MpPotion << endl;
}

// 프로그램 종료 메시지 함수
void EndGame()
{
	cout << "프로그램을 종료합니다." << endl;
}

// 잘못된 입력 메시지 함수
void InValidInput()
{
	cout << "입력이 잘못되었습니다." << endl;
}

int main()
{
	int status[] = { 0, 0, 0, 0 };
	SetHPMP(status);
	SetAtkDef(status);

	int HpPotionCount = 0;
	int MpPotionCount = 0;
	setPotion(5, &HpPotionCount, &MpPotionCount);

	cout << "* 포션이 지급되었습니다. (HP, MP 포션 각 5개)\n" <<
		"=============================================" << endl;

	bool IsItFirst = true;
	printDisplay(IsItFirst);
	IsItFirst = false;

	while (true) {
		int number;
		cin >> number;
		switch (number)
		{
		case 1:
			if (HasPotion(&HpPotionCount))
			{
				SetHPMpUp(20, &status[HP], &HpPotionCount);
				PrintHPMPChange(20, "HP", status[HP], HpPotionCount);
			} 
			else
			{
				NotifyPotionEmpty();
			}
			break;
		case 2:
			if (HasPotion(&MpPotionCount))
			{
				SetHPMpUp(20, &status[MP], &MpPotionCount);
				PrintHPMPChange(20, "MP", status[MP], MpPotionCount);
			}
			else
			{
				NotifyPotionEmpty();
			}
			break;
		case 3:
			SetAtkDefUp(2, &status[ATK]);
			PrintATKDEFChange(2, "공격력", status[ATK]);
			break;
		case 4:
			SetAtkDefUp(2, &status[DEF]);
			PrintATKDEFChange(2, "방어력", status[DEF]);
			break;
		case 5:
			printSt(status);
			break;
		case 6:
			LevelUp(&HpPotionCount, &MpPotionCount);
			LevelUpPrint(HpPotionCount, MpPotionCount);
			break;
		case 0:
			EndGame();
			return 0;
			break;
		default:
			InValidInput();
			break;
		}

		printDisplay(IsItFirst);
	}

	return 0;
}