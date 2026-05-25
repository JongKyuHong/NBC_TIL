#include <iostream>
#include <vector>
#include <string>

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
    
    const std::vector<std::string> getIngredients() const {
        return ingredients;
    }
private:
    std::string potionName;
    std::vector<std::string> ingredients; // 단일 재료에서 재료 '목록'으로 변경
};

// AlchemyWorkshop 클래스: 레시피 목록을 관리
class AlchemyWorkshop {
private:
    std::vector<PotionRecipe> recipes;

public:
    // addRecipe 메서드: 재료 목록(vector)을 매개변수로 받도록 수정
    void addRecipe(const std::string& name, const std::vector<std::string>& ingredients) {
        recipes.push_back(PotionRecipe(name, ingredients));
        std::cout << ">> 새로운 레시피 '" << name << "'이(가) 추가되었습니다." << std::endl;
    }

    // 모든 레시피 출력 메서드
    void displayAllRecipes() const {
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
            std::cout << std::endl;
        }
        std::cout << "---------------------------\n";
    }

    PotionRecipe searchRecipeByName(const std::string& name) const {
        for (const PotionRecipe& recipe : recipes) {
            if (recipe.getPotionName() == name) {
                return recipe;
            }
        }
        return PotionRecipe();
    }

    std::vector<PotionRecipe> searchRecipeByIngredient(const std::string& ingredient) {
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
};

int main() {
    AlchemyWorkshop myWorkshop;

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
            }
            else {
                std::cout << ">> 재료가 입력되지 않아 레시피 추가를 취소합니다." << std::endl;
            }

        }
        else if (choice == 2) {
            myWorkshop.displayAllRecipes();

        }
        else if (choice == 3) {
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
            }
            else {
                std::cout << "================================\n" <<
                    "검색 결과 없음\n" <<
                    "================================" <<
                    std::endl;
            }
        }
        else if (choice == 4) {
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
            }
            else {
                std::cout << "검색 결과 없음" << std::endl;
            }
        }
        else if (choice == 5) {
            std::cout << "공방 문을 닫습니다..." << std::endl;
            break;
        }
        else {
            std::cout << "잘못된 선택입니다. 다시 시도하세요." << std::endl;
        }
    }

    return 0;
}