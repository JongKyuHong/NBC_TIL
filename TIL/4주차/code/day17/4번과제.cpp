#include <iostream>
#include <vector>
#include <string>
#include <map>

// DTO
struct RecipeDisplayData {
    std::string potionName;
    std::vector<std::string> ingredients;
    int stock;

    RecipeDisplayData(std::string name, std::vector<std::string> ingr, int s)
        : potionName(name), ingredients(ingr), stock(s) {
    }
};

// PotionRecipe 클래스: 재료 목록을 vector<string>으로 변경
class PotionRecipe {
public:
    PotionRecipe() : potionName("") {}
    // 생성자: 재료 목록을 받아 초기화하도록 수정
    PotionRecipe(const std::string& name, const std::vector<std::string>& ingredients)
        : potionName(name), ingredients(ingredients) {
    }

    const std::string getPotionName() const {
        return potionName;
    }

    const std::vector<std::string>& getIngredients() const {
        return ingredients;
    }
private:
    std::string potionName;
    std::vector<std::string> ingredients; // 단일 재료에서 재료 '목록'으로 변경
};

class StockManager {
public:
    StockManager() {}
    StockManager(const std::string& name) {
        initializeStock(name);
    };

    void initializeStock(const std::string& potionName) {
        potionStock[potionName] = MAX_STOCK;
    }

    bool dispensePotion(const std::string& potionName) {
        auto it = potionStock.find(potionName);

        if (it == potionStock.end() || it->second <= 0) {
            return false;
        }

        it->second--;
        return true;
    }

    bool returnPotion(const std::string& potionName) {
        auto it = potionStock.find(potionName);

        if (it == potionStock.end() || it->second >= MAX_STOCK) {
            return false;
        }

        it->second++;
        return true;
    }

    int getStock(const std::string& potionName) {
        auto it = potionStock.find(potionName);
        return it != potionStock.end() ? it->second : 0;
    }
private:
    std::map<std::string, int> potionStock;
    const int MAX_STOCK = 3;
};

class RecipeManager {
private:
    std::vector<PotionRecipe> recipes;
public:
    RecipeManager() {}

    PotionRecipe* addRecipe(std::string& name, std::vector<std::string> ingredients) {
        recipes.push_back(PotionRecipe(name, ingredients));

        return &recipes[recipes.size()-1];
    }

    PotionRecipe* findRecipeByName(const std::string& name) {
        for (PotionRecipe& recipe : recipes) {
            if (recipe.getPotionName() == name) {
                return &recipe;
            }
        }
        return nullptr;
    }

    std::vector<PotionRecipe*> findRecipeByIngredient(std::string ingredient) {
        std::vector<PotionRecipe*> res;
        for (PotionRecipe& recipe : recipes) {
            const std::vector<std::string>& ingredients = recipe.getIngredients();
            for (const std::string& ingredient_ : ingredients) {
                if (ingredient == ingredient_) {
                    res.push_back(&recipe);
                    break;
                }
            }
        }
        return res;
    }

    const std::vector<PotionRecipe>& getAllRecipes() {
        return recipes;
    }

   
};

class AlchemyConsoleView {
public:
    AlchemyConsoleView() {}
    int printMainMenu() {
        std::cout << "⚗️ 연금술 공방 관리 시스템" << std::endl;
        std::cout << "1. 레시피 추가" << std::endl;
        std::cout << "2. 모든 레시피 출력" << std::endl;
        std::cout << "3. 이름으로 레시피 검색" << std::endl;
        std::cout << "4. 재료로 레시피 검색" << std::endl;
        std::cout << "5. 공병 반납" << std::endl;
        std::cout << "6. 종료" << std::endl;
        std::cout << "선택: ";

        return promptValidInput();
    }

    int promptValidInput() {
        int userInput;
        while (true) {
            std::cin >> userInput;

            if (std::cin.fail()) {
                std::cout << "잘못된 입력입니다. 숫자를 입력해주세요." << std::endl;
                std::cin.clear();
                std::cin.ignore(10000, '\n');
                continue;
            }
            break;
        }
        return userInput;
    }

    std::string promptPotionName() {
        std::string name;
        std::cout << "물약 이름: ";
        std::cin.ignore(10000, '\n');
        std::getline(std::cin, name);

        return name;
    }

    std::vector<std::string> promptPotionRecipe() {
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

        return ingredients_input;
    }

    void printAllRecipes(const std::vector<RecipeDisplayData>& data) {
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
            std::cout << " | 남은 재고 : " << data[i].stock << "개\n";
        }
        std::cout << "---------------------------" << std::endl;
    }

    std::string promptForSearch(const std::string& target) {
        std::string InpName;
        std::cout << "검색할 포션" << target << "을 입력해주세요 : ";
        std::cin.ignore(10000, '\n');
        std::getline(std::cin, InpName);
        return InpName;
    }

    void printSearchNotFound() {
        std::cout << "================================\n" <<
            "검색 결과 없음\n" <<
            "================================" <<
            std::endl;
    }

    void printSearchResult(const PotionRecipe& recipe, int stock) {
        std::cout << "================================\n" << 
            "이름 : " << recipe.getPotionName() <<
            "\n재고: " << stock << "개" <<
            "\n재료 : \n";
        const std::vector<std::string>& ingredients = recipe.getIngredients();
        for (int i = 0; i < ingredients.size(); i++) {
            std::cout << (i + 1) << ". " << ingredients[i] << "\n";
        }
        std::cout << std::endl;
    }
    
    bool promptyGetDispence() {
        std::cout << "\n 이 물약을 지급받겠습니다? (Y/N)";
        char answer;
        std::cin >> answer;
        std::cin.ignore(10000, '\n');
        return (answer == 'Y' || answer == 'y');
    }

    void printSuccessDispense(std::string name, int stock) {
        std::cout << "\n 포션 " << name << "이(가) 지급되었습니다.\n"
        << "남은 재고 : " << stock << "개";
    }

    // 지급받은 포션 출력함수
    void PrintDispensedPotion(std::string potionName, int stock) {
        std::cout << "이름 : " << potionName << " 개수 : " << stock << std::endl;
    }

    void printSuccessReturn(std::string potionName, int stock) {
        std::cout << "포션 " << potionName << "의 공병이 성공적으로 반납되었습니다.\n"
            << "남은 재고 : " << stock << std::endl;
    }

    void printMessage(const std::string& inp) {
        std::cout << inp << std::endl;
    }
};

// AlchemyWorkshop 클래스: 레시피 목록을 관리
class AlchemyWorkshop {
private:
    RecipeManager recipeManager;
    StockManager stockManager;
    AlchemyConsoleView alchemyView;
    std::map<std::string, int> myPotions;

public:
    AlchemyWorkshop() {}

    void addRecipe(std::string& name, std::vector<std::string>& ingredients) {
        recipeManager.addRecipe(name, ingredients);
        stockManager.initializeStock(name);
        alchemyView.printMessage(">> 새로운 레시피 '" + name + "'이(가) 추가되었습니다.");
    }

    // 모든 레시피 출력 메서드
    void displayAllRecipes() {
        const std::vector<PotionRecipe>& recipes = recipeManager.getAllRecipes();

        // 출력용 데이터 준비 (재고 정보 포함)
        std::vector<RecipeDisplayData> displayData;

        for (const auto& recipe : recipes) {
            std::string name = recipe.getPotionName();
            int stock = stockManager.getStock(name);
            displayData.emplace_back(name, recipe.getIngredients(), stock);
        }

        alchemyView.printAllRecipes(displayData);
    }

    void play() {
        while (true) {
            int choice = alchemyView.printMainMenu();

            if (choice == 1) {
                // 여러 재료를 입력받기 위한 로직
                std::string name = alchemyView.promptPotionName();
                std::vector<std::string> ingredients = alchemyView.promptPotionRecipe();

                // 입력받은 재료가 하나 이상 있을 때만 레시피 추가
                if (!ingredients.empty()) {
                    addRecipe(name, ingredients);
                } else {
                    alchemyView.printMessage("재료가 입력되지 않아 레시피 추가를 취소합니다.");
                }
            } else if (choice == 2) {
                displayAllRecipes();
            } else if (choice == 3) {
                // 이름 검색
                std::string target = "이름";
                std::string InpName = alchemyView.promptForSearch(target);

                PotionRecipe* result = recipeManager.findRecipeByName(InpName);
                if (result == nullptr) {
                    alchemyView.printSearchNotFound();
                    continue;
                } 
                int stock = stockManager.getStock(InpName);
                alchemyView.printSearchResult(*result, stock);

                if (stock > 0) {
                    bool isGetPotion = alchemyView.promptyGetDispence();

                    if (isGetPotion) {
                        bool success = stockManager.dispensePotion(InpName);
                        if (success) {
                            int currentStock = stockManager.getStock(InpName);
                            alchemyView.printSuccessDispense(InpName, currentStock);
                            myPotions[InpName]++;
                        } else {
                            alchemyView.printMessage("포션 지급에 실패하였습니다.");
                        }
                    } else {
                        alchemyView.printMessage("포션 지급을 받지않습니다.");
                    }
                }
            } else if (choice == 4) {
                // 재료로 검색
                std::string target = "재료";
                std::string InpName = alchemyView.promptForSearch(target);

                std::vector<PotionRecipe*> result = recipeManager.findRecipeByIngredient(InpName);

                if (result.size()) {
                    for (int i = 0; i < result.size(); i++) {
                        std::string potionName = result[i]->getPotionName();
                        int stock = stockManager.getStock(potionName);
                        alchemyView.printSearchResult(*result[i], stock);
                        if (stock > 0) {
                            bool isGetPotion = alchemyView.promptyGetDispence();

                            if (isGetPotion) {
                                bool success = stockManager.dispensePotion(potionName);
                                if (success) {
                                    int currentStock = stockManager.getStock(potionName);
                                    alchemyView.printSuccessDispense(potionName, currentStock);
                                    myPotions[potionName]++;
                                } else {
                                    alchemyView.printMessage("포션 지급에 실패하였습니다.");
                                }
                            } else {
                                alchemyView.printMessage("포션 지급을 받지않습니다.");
                            }
                        }
                    }
                } else {
                    alchemyView.printSearchNotFound();
                }
            }  else if (choice == 5) {
                // 공병 반납
                if (myPotions.empty()) {
                    alchemyView.printMessage("지급받은 포션이 없습니다.");
                    continue;
                }
                
                for (auto& potion : myPotions) {
                    alchemyView.PrintDispensedPotion(potion.first, potion.second);
                }
                
                std::string InpName = alchemyView.promptPotionName();
                auto it = myPotions.find(InpName);
                if (it != myPotions.end()) {
                    bool success = stockManager.returnPotion(InpName);
                    if (success) {
                        int currentStock = stockManager.getStock(InpName);
                        alchemyView.printSuccessReturn(InpName, currentStock);
                        myPotions[InpName]--;
                        if (myPotions[InpName] == 0) {
                            myPotions.erase(InpName);
                        }
                    } else {
                        alchemyView.printMessage("포션 반납에 실패했습니다.");
                    }
                } else {
                    alchemyView.printMessage("해당 포션은 지급받지 않았습니다.");
                }
                
            } else if (choice == 6) {
                alchemyView.printMessage("공방 문을 닫습니다...");
                break;
            } else {
                alchemyView.printMessage("잘못된 선택입니다. 다시 시도하세요.");
            }
        }
    }
};

int main() {
    AlchemyWorkshop myWorkshop;
    myWorkshop.play();

    return 0;
}