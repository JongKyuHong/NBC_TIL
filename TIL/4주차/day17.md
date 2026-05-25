# C++

## 4번 과제

도전기능을 붙이려고 하니 재사용도 많고 명확하게 프로젝트 구조를 인지를 못하는것 같아서 처음부터 다시 설계를 하고 진행하기로 마음먹었다.

### 체크포인트

- [x] 지급 및 반환
  - [x] 물약이름으로 재고 검색, 재고 1개 이상일 경우 모험가에게 지급할 수 있어야함
  - [x] 레시피 재료로 물약들의 재고 검색, 재고 1개이상인 경우 지급할 수 있어야함
  - [x] 모험가에게 지급한 물약 공병 반환받기
  - [x] 최대 재고 3개, 반환 시 3개 초과 불가하다.
- [x] 어떤 의도로 코드를 구현했는지 정리
- [x] 수정 시 기존 코드의 수정을 최소화하면서 수정할 수 있는 방안이 있는지 고려했는가
- [x] SOLID 원칙을 잘 준수하고 있는가

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

### View의 책임??

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

- 사용자가 어떤 행동을 하는지 번호로 입력받는 부분에서 숫자가 아닌경우를 검증하는 로직이 있었는데 이를 컨트롤러 역할인 workshop에 두는게 나은지 아니면 View로 빼는게 좋을지 고민이 되었다.

> 해결방법

AI의 도움을 받아서 구분을 확실히 짓게 되었다.

```
🎯 핵심 구분: UI 레벨 vs 비즈니스 로직 레벨
✅ View가 해도 되는 것 (UI 레벨 검증)
**"사용자 인터페이스 차원의 입력 오류"**는 View의 책임입니다.

❌ View가 하면 안 되는 것 (비즈니스 로직 검증)
**"게임 규칙에 따른 검증"**은 Controller나 Model의 책임입니다.
```

- 정리하자면 View의 역할은 화면에 콘솔을 찍어주는 역할만 하기때문에 게임 규칙에 대한 검증은 View가 모르는게 낫고, 지금처럼 입력을 검증하는 역할정도는 View에서 처리해도 상관없다는 답변을 받았다.
- 그래서 구조를 PrintMainMenu로 입력창을 띄우고 -> PromptValidInput() 함수로 유저의 인풋을 받는데 여기다가 검증 로직을 넣어서 검증하는식으로 구현하기로 했다.
- 유저의 입력을 받을때 Prompt라는 단어를 많이 쓴다고 해서 변경했다.

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

        return PromptValidInput();
    }

    int PromptValidInput() {
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

### displayAllRecipes()

- 기존에 AlchemyWorkshop에서 출력을 담당하던 함수를 View클래스로 옮기는 작업을 하려 했다.

```c++
// 모든 레시피 출력 메서드
void displayAllRecipes() {
    const std::vector<PotionRecipe>& recipes = recipeManager.getAllRecipes();
    if (recipes.empty()) {
        std::cout << "아직 등록된 레시피가 없습니다." << std::endl;
        return;
    }

    std::cout << "\n--- [ 전체 레시피 목록 ] ---" << std::endl;
    for (size_t i = 0; i < recipes.size(); ++i) {
        std::cout << "- 물약 이름: " << recipes[i].getPotionName() << std::endl;
        std::cout << "  > 필요 재료: ";
        std::vector<std::string> ingredients = recipes[i].getIngredients();
        // 재료 목록을 순회하며 출력
        for (size_t j = 0; j < ingredients.size(); ++j) {
            std::cout << ingredients[j];
            // 마지막 재료가 아니면 쉼표로 구분
            if (j < ingredients.size() - 1) {
                std::cout << ", ";
            }
        }
        std::cout << "남은 재고 : " << stockManager.getStock(recipes[i].getPotionName()) << "개";
    }
    std::cout << "---------------------------" << std::endl;
}
```

- 허나 기존 코드를 보면 recipeManager, stockManager를 사용하고 있기 때문에 이를 참조로 View클래스에 넘겨주면 View에서 모델의 존재를 알아버리는 문제가 있었다.

> 해결 방법

- 이는 구조체를 활용해서 DTO를 정의 했다.
- DTO : 계층 간 데이터 전송을 위한 순수 데이터 객체

```c++
struct RecipeDisplayData {
    std::string potionName;
    std::vector<std::string> ingredients;
    int stock;

    RecipeDisplayData(std::string name, std::vector<std::string> ingr, int s)
        : potionName(name), ingredients(ingr), stock(s) {
    }
};
```

- 구초제에는 이름, 재료, 갯수를 넣어주었다.

- **displayAllRecipes**

```c++
class AlchemyWorkshop {
    ...
    void displayAllRecipes() {
        const std::vector<PotionRecipe>& recipes = recipeManager.getAllRecipes();

        // 출력용 데이터 준비 (재고 정보 포함)
        std::vector<RecipeDisplayData> displayData;

        for (const auto& recipe : recipes) {
            std::string name = recipe.getPotionName();
            int stock = stockManager.getStock(name);
            displayData.emplace_back(name, recipe.getIngredients(), stock);
        }

        alchemyView.PrintAllRecipes(displayData);
    }
    ...
}
```

- controller즉 workshop 클래스에서 model인 recipeManager을 통해 전체 레시피를 받아오고, 미리 제작한 DTO의 규격에 맞게 데이터들을 불러와서 저장한 후 View의 PrintAllRecipes 멤버함수를 호출해서 출력해주었다.

- **PrintAllRecipes**

```c++
void PrintAllRecipes(const std::vector<RecipeDisplayData>& data) {
    if (data.empty()) {
        std::cout << "아직 등록된 레시피가 없습니다." << std::endl;
        return;
    }

    std::cout << "\n--- [ 전체 레시피 목록 ] ---" << std::endl;
    for (size_t i = 0; i < data.size(); ++i) {
        std::cout << "- 물약 이름: " << data[i].potionName << std::endl;
        std::cout << "  > 필요 재료: ";
        std::vector<std::string> ingredients = data[i].ingredients;
        // 재료 목록을 순회하며 출력
        for (size_t j = 0; j < ingredients.size(); ++j) {
            std::cout << ingredients[j];
            // 마지막 재료가 아니면 쉼표로 구분
            if (j < ingredients.size() - 1) {
                std::cout << ", ";
            }
        }
        std::cout << "남은 재고 : " << data[i].stock << "개";
    }
    std::cout << "---------------------------" << std::endl;
}
```

### 이름 검색 분리하기

- 기존의 이름 검색 메서드를 workshop클래스에서 지우고 play()함수에서 3번이 들어오면 바로 recipeManager에 있는 이름검색 함수를 호출하는식으로 변경했다.

- play()

```c++
else if (choice == 3) {
    // View에 존재하는 입출력 함수 사용자가 입력한 이름을 반환
    std::string InpName = alchemyView.PromptPotionNameForSearch();

    // 사용자가 입력한 이름으로 직접 매니저에서 로직을 돌고 결과를 들고옴 포인터 반환
    PotionRecipe* result = recipeManager.findRecipeByName(InpName);
    if (result == nullptr) {
        // 검색어가 없으면 못찾았다는 View출력
        alchemyView.PrintSearchNotFound();
    } else {
        // 검색어가 있으면 해당 물약의 재고를 가져오고
        int stock = stockManager.getStock(InpName);
        // View에서 출력해준다.
        alchemyView.PrintSearchResult(*result, stock);
    }
```

### 재료 검색하기

- 이름 검색하기와 어느정도 겹치는 부분이 있어서 함수를 최대한 재활용하고자 했다.
- **PromptPotionNameForSearch**

```c++
std::string PromptPotionNameForSearch() {
    std::string InpName;
    std::cout << "검색할 포션 이름을 입력해주세요 : ";
    std::cin.ignore(10000, '\n');
    std::getline(std::cin, InpName);
    return InpName;
}
// 기존 위에서 아래로
// target으로 이름/재료를 입력받음
std::string PromptForSearch(const std::string& target) {
    std::string InpName;
    std::cout << "검색할 포션" << target << "을 입력해주세요 : ";
    std::cin.ignore(10000, '\n');
    std::getline(std::cin, InpName);
    return InpName;
}
```

- **PrintSearchResult**
- 해당 함수는 이름검색한 결과를 뷰에서 보여주는 역할을하는데 이를 이름, 재료검색결과 함수로 나눠서 만들려고 할때 문제가 생겼다.

```
void PrintNameSearchResult(const PotionRecipe& recipe, int stock) {
    std::cout << "================================\n" <<
        "이름 : " << recipe.getPotionName() << "\n재료 : \n";
    std::vector<std::string> ingredients = recipe.getIngredients();
    for (int i = 0; i < ingredients.size(); i++) {
        std::cout << (i + 1) << ". " << ingredients[i] << "\n";
    }
    std::cout << std::endl;
}

void PrintIngredientsSearchResult(const std::vector<PotionRecipe>& result) {
    for (int i = 0; i < result.size(); i++) {
        std::cout << (i + 1) << "번 검색 결과\n";

        PrintNameSearchResult(result[i], stock)
    }
}
```

- 만약 재료검색을 했을때 결과가 PrintIngredientsSearchResult이 함수로 넘어오게 되는데 수량을 PrintNameSearchResult로 보내려면 결국에는 View에서 stockManager를 써야했다.

> 해결방법

- 기존처럼 PrintSearchResult로 바꾸고 play()에서 for문으로 여러번 호출하는 식으로 변경했다.

```c++
std::string target = "재료";
std::string InpName = alchemyView.PromptForSearch(target);

std::vector<PotionRecipe> result = recipeManager.findRecipeByIngredient(InpName);

if (result.size()) {
    for (int i = 0; i < result.size(); i++) {
        int stock = stockManager.getStock(result[i].getPotionName());
        alchemyView.PrintSearchResult(result[i], stock);
    }
} else {
    alchemyView.PrintSearchNotFound();
}
```

### 종료 메시지

```c++
else if (choice == 5) {
    std::cout << "공방 문을 닫습니다..." << std::endl;
    break;
} else {
    std::cout << "잘못된 선택입니다. 다시 시도하세요." << std::endl;
}
```

- 종료를 선택했거나 잘못된 입력을 할때 나오는 텍스트 처리가 필요했다.

```
void PrintMessage(const std::string& inp) {
    std::cout << inp << std::endl;
}
```

- 둘다 해당 함수로 처리했다.
- 전용함수를 만들어도 되겠지만 지금 상태에서는 함수하나로 처리해도 괜찮다고 판단했다.

### 중간 점검

#### map의 임의접근 보다는 find를 안전하게 사용

```c++
int getStock(const std::string& potionName) {
    return potionStock[potionName] ? potionStock[potionName] : 0; // map은 potionName이 없으면 0으로 생성해버림, find로 대체해야함
}
...
// 수정 후
int getStock(const std::string& potionName) {
    auto it = potionStock.find(potionName);
    return it != potionStock.end() ? it->second : 0;
}
```

#### WorkShop클래스에 std남아있음

```c++
std::cout << ">> 새로운 레시피 '" << name << "'이(가) 추가되었습니다." << std::endl; // 남아있던 std문을 PrintMessage로 대체
...
// 수정 후
alchemyView.PrintMessage(">> 새로운 레시피 '" + name + "'이(가) 추가되었습니다.");
```

#### 비즈니스 로직은 View에있으면 안됨

```c++
// 해당 조건문은 play()함수로
if (ingredients_input.size() == 0) {
    std::cout << ">> 재료가 입력되지 않아 레시피 추가를 취소합니다." << std::endl;
    return {};
}
return ingredients_input;

...
// 수정 후
if (!ingredients.empty()) {
    addRecipe(name, ingredients);
} else {
    alchemyView.PrintMessage("재료가 입력되지 않아 레시피 추가를 취소합니다.");
}
```

#### 포인터의 중요성

```c++
std::vector<PotionRecipe> findRecipeByIngredient(std::string ingredient) {
    std::vector<PotionRecipe> res;
    for (const PotionRecipe& recipe : recipes) {
        std::vector<std::string> ingredients = recipe.getIngredients();
        for (const std::string& ingredient_ : ingredients) {
            if (ingredient == ingredient_) {
                res.push_back(recipe);
                break;
            }
        }
    }
    return res;
}
```

- 기존에는 검색어에 걸리는 레시피들을 res.push_back으로 recipe를 받아서 배열을 넘겨주었는데 push_back을 하는 과정에서 복사가 일어나서 성능이 저하됨을 확인하였다.

```c++
std::vector<PotionRecipe*> findRecipeByIngredient(std::string ingredient) {
    std::vector<PotionRecipe*> res;
    for (const PotionRecipe& recipe : recipes) {
        std::vector<std::string> ingredients = recipe.getIngredients();
        for (const std::string& ingredient_ : ingredients) {
            if (ingredient == ingredient_) {
                res.push_back(&recipe);
                break;
            }
        }
    }
    return res;
}
```

- push_back에서 오류가 났다. 왜냐하면 res는 그냥 포인터를 받는 벡터이고, &recipe은 const가 붙은 포인터였다.
- for문에서 const를 빼던 res선언에서 붙여주던 하나를 해야하는데 for문에서 const를 빼기로 했다.
- 이렇게 변경하니 이제 다른곳에서 문제가 생겼다.

```c++
std::vector<PotionRecipe*> result = recipeManager.findRecipeByIngredient(InpName);

if (result.size()) {
    for (int i = 0; i < result.size(); i++) {
        int stock = stockManager.getStock(*result[i].getPotionName()); // result 에러
        alchemyView.printSearchResult(*result[i], stock);
}
```

- result를 포인터 배열로 잘 변경했는데 result[i]에서 에러가 났다.
- 이유는 연산자 우선순위때문에 그렇다고 한다.
  - [] : 가장높음
  - .(멤버접근)
  - \*(역참조) : 가장 낮음
- 이 때문에 \*(result[i].getPotionName()) // 이런식으로 해석되고 있었다고 한다.
- -> 화살표로 변경해서 해결하였다.

이외에도 자잘한 const, & 안붙여서 복사가 일어나는곳이 많은등 많은 문제가있었다..

### 포션 지급 기능 만들기

- 우선 StockManager의 dispensePotion함수부터 만들었다.

```c++
bool dispensePotion(const std::string& potionName) {
    auto it = potionStock.find(potionName);

    if (it == potionStock.end() || it->second <= 0) {
        return false;
    }

    it->second--;
    return true;
}
```

- 밖에서 검증을 하고 들어올거지만 혹시라도 포션이 없거나, 재고가 없을때 false를 반환해주었다.
- 모델을 호출하는 친구는 컨트롤러다 컨트롤러로 가서 얘를 불러오자

```c++
if (stock > 0) {
    bool isGetPotion = alchemyView.promptyGetDispence();

    if (isGetPotion) {
        bool success = stockManager.dispensePotion(InpName);
        if (success) {
            int currentStock = stockManager.getStock(InpName);
            alchemyView.printSuccessDispense(InpName, currentStock);
        } else {
            alchemyView.printMessage("포션 지급에 실패하였습니다.");
        }
    } else {
        alchemyView.printMessage("포션 지급에 실패하였습니다.");
    }
}
...
bool promptyGetDispence() {
    std::cout << "\n 이 물약을 지급받겠습니다? (Y/N)";
    char answer;
    std::cin >> answer;
    std::cin.ignore(10000, '\n');
    return (answer == 'Y' || answer == 'y');
}
```

- 재료검색도 이와같이 만들었다.

### 공병 반납

- 내가 지급받지 않은 포션은 반납할 수 없어야하고, 공병 반납을 선택했을때 내가 지급받은 포션 리스트가 나오면 좋겠다고 생각했다.
- 우선 workshop클래스에 지급받은 공병을 저장하는 벡터를 만들었다.

```c++
bool returnPotion(const std::string& potionName) {
    auto it = potionStock.find(potionName);

    if (it == potionStock.end() || it->second >= MAX_STOCK) {
        return false;
    }

    it->second++;
    return true;
}
```

- returnPotion의 경우도 bool형 함수로 변경하고, dispensePotion과 비슷하게 만들었다.
- MAX_STOCK 조건만 추가
- AI한테 물어보니 실패 이유를 Model이 판단할 수 있어서 이렇게 짜는게 더 낫다고 한다.
  - Max_stock은 모델의 내부 규칙
  - 모델이 불변성 보장
  - 그냥 이렇게 짠건데 얻어걸림
