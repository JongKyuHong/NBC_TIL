// https://www.acmicpc.net/problem/20920

#include <algorithm>
#include <iostream>
#include <unordered_map>

using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(NULL);

    // 많이 나올수록 앞
    // 길수록 앞
    // 사전순으로 앞설수록 앞
    int N, M;
    cin >> N >> M;
    unordered_map<string, int> words;
    
    for (int i = 0; i < N; i++)
    {
        string word;
        cin >> word;
        if (word.length() >= M)
        {
            words[word]++;    
        }
    }
    
    vector<pair<string, int>> vec;
    
    for (const auto& [word, count] : words)
    {
        vec.push_back({word, count});    
    }
    
    sort(vec.begin(), vec.end(), [](const pair<string, int>& a, const pair<string, int>& b)
    {
        if (a.second != b.second)
        {
            return a.second > b.second;
        } else if (a.first.length() != b.first.length())
        {
            return a.first.length() > b.first.length();
        } else
        {
            return a.first < b.first;
        }
    });
    
    for (const auto& [word, count] : vec)
    {
        cout << word << "\n";
    }
    
    return 0;
}