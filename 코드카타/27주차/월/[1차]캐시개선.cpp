// https://school.programmers.co.kr/learn/courses/30/lessons/17680?language=cpp#

#include <string>
#include <vector>
#include <unordered_map>
#include <algorithm>
#include <cctype>
#include <list>
#include <iostream>

using namespace std;

int solution(int cacheSize, vector<string> cities) {
    if (cacheSize == 0) {
        return cities.size() * 5;
    }
    int time = 0;
    
    list<string> lruList;
    unordered_map<string, list<string>::iterator> cache;
    
    for (string& city : cities){
        transform(city.begin(), city.end(), city.begin(), ::tolower);

        // find대신 count를 쓸수도있다.
        if (cache.count(city)){
            // 있으면 캐시 히트
            lruList.erase(cache[city]);
            lruList.push_front(city);
            cache[city] = lruList.begin();
            time++;
        } else {
            if (cache.size() >= cacheSize){
                string lastCity = lruList.back();
                cache.erase(lastCity);
                lruList.pop_back();
            }
            lruList.push_front(city);
            cache[city] = lruList.begin();
            
            time+=5;
        }
    }
    
    return time;
}