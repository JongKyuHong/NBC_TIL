// https://school.programmers.co.kr/learn/courses/30/lessons/17680?language=cpp#

#include <string>
#include <vector>
#include <unordered_map>
#include <algorithm>
#include <cctype>
#include <iostream>

using namespace std;

int solution(int cacheSize, vector<string> cities) {
    if (cacheSize == 0) {
        return cities.size() * 5;
    }
    int time = 0;
    
    int cacheNum = 1;
    unordered_map<string, int> cache;
    for (string& city : cities){
        transform(city.begin(), city.end(), city.begin(), ::tolower);
        if (cache.find(city) != cache.end()){
            cache[city] = cacheNum++;
            time++;
        } else {
            // CacheMiss인경우
            if (cache.size() >= cacheSize){
                string target = "";
                int targetNum = 100001;
                for (const auto& [key, value] : cache){
                    if (targetNum > value){
                        targetNum = value;
                        target = key;
                    }
                }

                cache.erase(target);
            }
            cache[city] = cacheNum++;
            time+=5;
        }    
    }
    
    return time;
}