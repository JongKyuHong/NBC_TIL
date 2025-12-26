using namespace std;

long long solution(int price, int money, int count)
{
    long long answer = 0;
    int tmp = price;
    for (int i = 0; i < count; i++){
        answer += tmp;
        tmp += price;
    }
    
    if (answer > money){
        return answer - money;
    } else {
        return 0;
    }
    
    
}