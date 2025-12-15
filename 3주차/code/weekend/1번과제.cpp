// Copyright 2025 JongKyu.

#include <iostream>
#include <string>
#include <limits>

using namespace std;  // NOLINT

enum { HP, MP, ATK, DEF };

// 잘못된 입력 걸러내는 하뭇
void ClearBadInput() {
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

// HP/MP 설정 함수
void SetHPMP(int* statusPtr) {
    while (true) {
        cout << "HP와 MP를 입력해주세요 : ";
        int InpHP;
        int InpMP;
        if (!(cin >> InpHP >> InpMP)) {
            ClearBadInput();
            cout << "숫자를 입력해주세요.\n";
            continue;
        }

        if (InpHP <= 50 || InpMP <= 50) {
            cout << "HP나 MP의 값이 너무 작습니다. 다시 입력해주세요.\n";
            continue;
        } else {
            statusPtr[HP] = InpHP;
            statusPtr[MP] = InpMP;
            return;
        }
    }
}

// 공격력/방어력 설정 함수
void SetAtkDef(int* statusPtr) {
    while (true) {
        cout << "공격력과 방어력을 입력해주세요 : ";
        int InpAtk;
        int InpDef;

        if (!(cin >> InpAtk >> InpDef)) {
            ClearBadInput();
            cout << "숫자를 입력해주세요.\n";
            continue;
        }

        if (InpAtk <= 0 || InpDef <= 0) {
            cout << "공격력이나 방어력이 너무 작습니다. 다시 입력해주세요.\n";
        } else {
            statusPtr[2] = InpAtk;
            statusPtr[3] = InpDef;
            return;
        }
    }
}

// 포션 개수 설정 함수
void SetPotion(const int& count, int* p_HPPotion, int* p_MPPotion) {
    *p_HPPotion = count;
    *p_MPPotion = count;
}

// 포션 설정 메시지 함수
void PrintSetPotion() {
  cout << "* 포션이 지급되었습니다. (HP, MP 포션 각 5개)\n"
       << "=============================================" << endl;
}

// 시스템 메시지를 출력하는 함수, 항상 메뉴가 나오도록 했다.
void PrintDisplay() {
    cout << "<스탯 관리 시스템>\n" <<
        "1. HP UP\n2. MP UP\n3. 공격력 UP\n4. 방어력 UP" <<
      "\n5.현재 능력치\n6.Level UP\n0.나가기\n ";
    cout << "번호를 선택해주세요: ";
}

// 포션을 가지고있는지 확인하는 함수
bool HasPotion(int* Potion) {
    if (*Potion <= 0) {
        return false;
    }
    return true;
}

// 포션이 없는경우 상태 메시지를 출력하는 함수
void NotifyPotionEmpty() {
    cout << "포션이 부족합니다." << endl;
}

/*
void SetHpUp(const int& value, int* statusPtr, int* Potion) {
    *statusPtr += value;
    *Potion -= 1;
}
*/

// 포션을 1개 차감하는 함수
void SetHPMpUp(const int& value, int* statusPtr, int* Potion) {
    *statusPtr += value;
    *Potion -= 1;
}

// 공격력/방어력을 늘려주는 함수
void SetAtkDefUp(const int& value, int* statusPtr) {
    *statusPtr = value * (*statusPtr);
}

/*
 void SetDefUp(const int& value, int* statusPtr)
 {
 	*statusPtr = value * (*statusPtr);
 }
 */

// HP/MP 변경 메시지 출력
void PrintHPMPChange(const int& value, const string& StName,
    int& CurrentSt, int& CurrentPotionCnt) {
    cout << "*" << StName << "가" << value << "증가되었습니다."
        << "포션이 1개 차감됩니다.\n"
        << "현재 " << StName << ": " << CurrentSt << "\n"
        << "남은 포션 수 : " << CurrentPotionCnt << endl;
}

// 공격력/방어력 변경 메시지 출력
void PrintATKDEFChange(const int& Multiply,
    const string& StName, int& CurrentSt) {
    cout << "*" << StName << "이 " << Multiply << "배로 증가되었습니다.\n" <<
        "현재 " << StName << ": " << CurrentSt << endl;
}

// 현재 스텟 출력 함수
void PrintSt(int* statusPtr) {
    cout << "* HP : " << statusPtr[0] <<
        ", MP : " << statusPtr[1] <<
        ", 공격력 : " << statusPtr[2] <<
        ", 방어력 : " << statusPtr[3] << endl;
}

// 레벨업 함수
// HP, MP포션을 하나씩 더 지급
void LevelUp(int* HpPotion, int* MpPotion) {
    *HpPotion += 1;
    *MpPotion += 1;
}

// 레벨업시 메시지 출력 함수
// 현재 포션수를 출력함
void LevelUpPrint(const int& HpPotion, const int& MpPotion) {
    cout << "* 레벨업! HP/MP 포션이 지급됩니다.\n" <<
        "남은 HP/MP 포션 수 : " << HpPotion << "/" << MpPotion << endl;
}

// 프로그램 종료 메시지 함수
void EndGame() {
    cout << "프로그램을 종료합니다." << endl;
}

// 잘못된 입력 메시지 함수
void InValidInput() {
    cout << "입력이 잘못되었습니다." << endl;
}

int main() {
    // 스텟 저장 배열
    // 0: HP, 1 : MP, 2 : 공격력, 3: 방어력
    int status[] = { 0, 0, 0, 0 };

    // HP/MP/공격력/방어력 설정
    SetHPMP(status);
    SetAtkDef(status);

    // HP/MP포션 개수 저장 변수
    int HpPotionCount = 0;
    int MpPotionCount = 0;
    // 포션 개수 설정
    SetPotion(5, &HpPotionCount, &MpPotionCount);
    PrintSetPotion();
    // 시스템 메시지 출력
    PrintDisplay();

    while (true) {
        int number;
        cin >> number;

        // 사용자에게 번호 입력을 받아서 처리
        switch (number) {
        case 1:
            // HP UP
            if (HasPotion(&HpPotionCount)) {
                SetHPMpUp(20, &status[HP], &HpPotionCount);
                PrintHPMPChange(20, "HP", status[HP], HpPotionCount);
            } else {
                NotifyPotionEmpty();
            }
            break;
        case 2:
            // MP UP
            if (HasPotion(&MpPotionCount)) {
                SetHPMpUp(20, &status[MP], &MpPotionCount);
                PrintHPMPChange(20, "MP", status[MP], MpPotionCount);
            } else {
                NotifyPotionEmpty();
            }
            break;
        case 3:
            // 공격력 UP
            SetAtkDefUp(2, &status[ATK]);
            PrintATKDEFChange(2, "공격력", status[ATK]);
            break;
        case 4:
            // 방어력 UP
            SetAtkDefUp(2, &status[DEF]);
            PrintATKDEFChange(2, "방어력", status[DEF]);
            break;
        case 5:
            // 현재 능력치
            PrintSt(status);
            break;
        case 6:
            // Level UP
            LevelUp(&HpPotionCount, &MpPotionCount);
            LevelUpPrint(HpPotionCount, MpPotionCount);
            break;
        case 0:
            // 나가기
            EndGame();
            return 0;
            break;
        default:
            InValidInput();
            break;
        }

        PrintDisplay();
    }

    return 0;
}
