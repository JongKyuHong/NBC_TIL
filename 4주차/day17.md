# C++

## 4번 과제

도전기능을 붙이려고 하니 재사용도 많고 명확하게 프로젝트 구조를 인지를 못하는것 같아서 처음부터 다시 설계를 하고 진행하기로 마음먹었다.

### 설계

MVC패턴을 따르기로 하였다.

```
📦 Model
 ├─ PotionRecipe - 물약 데이터 구조
 ├─ RecipeManager - 레시피 검색/추가/관리 로직
 └─ StockManager - 재고 관리 로직

🎮 Controller
 └─ AlchemyWorkshop - Model들을 조율하고 게임 루프 관리

🖥️ View
 └─ AlchemyConsoleView - 화면 출력 및 사용자 입력 담당
```

### 1. main함수 게임 로직 이전하기

```c++
void play() { // 메인함수에 있던 로직을 우선 복사/붙여넣기 했다.
    while (true) {
        std::cout << "⚗️ 연금술 공방 관리 시스템" << std::endl;
        std::cout << "1. 레시피 추가" << std::endl;
        std::cout << "2. 모든 레시피 출력" << std::endl;
        std::cout << "3. 이름으로 레시피 검색" << std::endl;
        std::cout << "4. 재료로 레시피 검색" << std::endl;
        std::cout << "5. 종료" << std::endl;
        std::cout << "선택: ";

        int choice;
        std::cin >> choice;

        if (std::cin.fail()) {
            std::cout << "잘못된 입력입니다. 숫자를 입력해주세요." << std::endl;
            std::cin.clear();
            std::cin.ignore(10000, '\n');
            continue;
        }

        if (choice == 1) {
            std::string name;
            std::cout << "물약 이름: ";
            std::cin.ignore(10000, '\n');
            std::getline(std::cin, name);

            // 여러 재료를 입력받기 위한 로직
            std::vector<std::string> ingredients_input;
            std::string ingredient;
            std::cout << "필요한 재료들을 입력하세요. (입력 완료 시 '끝' 입력)" << std::endl;

            while (true) {
                std::cout << "재료 입력: ";
                std::getline(std::cin, ingredient);

                // 사용자가 '끝'을 입력하면 재료 입력 종료
                if (ingredient == "끝") {
                    break;
                }
                ingredients_input.push_back(ingredient);
            }

            // 입력받은 재료가 하나 이상 있을 때만 레시피 추가
            if (!ingredients_input.empty()) {
                myWorkshop.addRecipe(name, ingredients_input);
            } else {
                std::cout << ">> 재료가 입력되지 않아 레시피 추가를 취소합니다." << std::endl;
            }

        } else if (choice == 2) {
            myWorkshop.displayAllRecipes();

        } else if (choice == 3) {
            std::string InpName;
            std::cout << "검색할 포션 이름을 입력해주세요 : ";
            std::cin.ignore(10000, '\n');
            std::getline(std::cin, InpName);

            PotionRecipe result = myWorkshop.searchRecipeByName(InpName);
            if (result.getPotionName().length()) {
                std::cout << "이름 : " << result.getPotionName() << "\n재료 : \n";

                std::vector<std::string> ingredients = result.getIngredients();
                for (int i = 0; i < ingredients.size(); i++) {
                    std::cout << (i + 1) << ". " << ingredients[i] << "\n";
                }
                std::cout << std::endl;
            } else {
                std::cout << "================================\n" <<
                    "검색 결과 없음\n" <<
                    "================================" <<
                    std::endl;
            }
        } else if (choice == 4) {
            std::string InpName;
            std::cout << "검색할 재료 이름을 입력해주세요 : ";
            std::cin.ignore(10000, '\n');
            std::getline(std::cin, InpName);

            std::vector<PotionRecipe> result = myWorkshop.searchRecipeByIngredient(InpName);
            if (result.size()) {
                for (int i = 0; i < result.size(); i++) {
                    std::cout << (i + 1) << "번 검색 결과\n"
                        << "이름 : " << result[i].getPotionName() << "\n재료 : \n";

                    std::vector<std::string> ingredients = result[i].getIngredients();
                    for (int j = 0; j < ingredients.size(); j++) {
                        std::cout << (j + 1) << ". " << ingredients[j] << "\n";
                    }
                    std::cout << std::endl;
                }
            } else {
                std::cout << "검색 결과 없음" << std::endl;
            }
        } else if (choice == 5) {
            std::cout << "공방 문을 닫습니다..." << std::endl;
            break;
        } else {
            std::cout << "잘못된 선택입니다. 다시 시도하세요." << std::endl;
        }
    }
}

int main() {
    AlchemyWorkshop myWorkshop;
    myWorkshop.play();

    return 0;
}
```

- AlchemyWorkshop 클래스에서 play() 멤버함수를 만들어서 그쪽으로 이전을 하였다.
- 우선 cout작업들은 모두 AlchemyConsoleView쪽으로 분리를 하기로 하였다.

```c++
class AlchemyWorkshop {
private:
    RecipeManager recipeManager;
    StockManager stockManager;
    AlchemyConsoleView alchemyView; // 멤버 변수에 인스턴스 추가
```

#### View의 책임??

```c++
int choice;
std::cin >> choice;

if (std::cin.fail()) {
    std::cout << "잘못된 입력입니다. 숫자를 입력해주세요." << std::endl;
    std::cin.clear();
    std::cin.ignore(10000, '\n');
    continue;
}
```

- 사용자가 어떤 행동을 하는지 번호로 입력받는 부분에서 숫자가 아닌경우를 검증하는 로직이 있었는데 이를 컨트롤러 역할인 workshop에 두는게 나은지 아니면 View로 빼는게 좋을지 고민되어 AI의 도움을 받았다.

```
🎯 핵심 구분: UI 레벨 vs 비즈니스 로직 레벨
✅ View가 해도 되는 것 (UI 레벨 검증)
**"사용자 인터페이스 차원의 입력 오류"**는 View의 책임입니다.

❌ View가 하면 안 되는 것 (비즈니스 로직 검증)
**"게임 규칙에 따른 검증"**은 Controller나 Model의 책임입니다.
```

- 정리하자면 View의 역할은 화면에 콘솔을 찍어주는 역할만 하기때문에 게임 규칙에 대한 검증은 View가 모르는게 낫고, 지금처럼 입력을 검증하는 역할정도는 View에서 처리해도 상관없다는 답변을 받았다.
- 그래서 구조를 PrintMainMenu로 입력창을 띄우고 -> getValidInput() 함수로 유저의 인풋을 받는데 여기다가 검증 로직을 넣어서 검증하는식으로 구현하기로 했다.

```c++
class AlchemyConsoleView {
public:
    AlchemyConsoleView() {}
    int printMainMenu() {
        std::cout << "⚗️ 연금술 공방 관리 시스템" << std::endl;
        std::cout << "1. 레시피 추가" << std::endl;
        std::cout << "2. 모든 레시피 출력" << std::endl;
        std::cout << "3. 이름으로 레시피 검색" << std::endl;
        std::cout << "4. 재료로 레시피 검색" << std::endl;
        std::cout << "5. 종료" << std::endl;
        std::cout << "선택: ";

        return getValidInput();
    }

    int getValidInput() {
        int userInput;
        while (true) {
            std::cin >> userInput;

            if (std::cin.fail()) {
                std::cout << "잘못된 입력입니다. 숫자를 입력해주세요." << std::endl;
                std::cin.clear();
                std::cin.ignore(10000, '\n');
                continue;
            }
        }
        return userInput;
    }
};

void play() {
    while (true) {
        int choice = alchemyView.printMainMenu();
        ...
```

- 이런식으로 구현을 바꾸게 되었다.
