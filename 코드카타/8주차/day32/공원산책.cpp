#include <string>
#include <vector>
#include <iostream>

using namespace std;

int startR = 0;
int startC = 0;
int N;
int M;

bool move(char dir, int value, vector<string>& vec){
    if (dir == 'N'){
        int index = 1;
        while (index <= value){
            if (startR-index < 0 || vec[startR-index][startC] == 'X' ){
                return false;
            }
            index++;
        }
    } else if (dir == 'S'){
        int index = 1;
        while (index <= value){
            if (startR+index >= N || vec[startR+index][startC] == 'X' ){
                return false;
            }
            index++;
        }
    } else if (dir == 'W'){
        int index = 1;
        while (index <= value){
            if (startC-index < 0 || vec[startR][startC-index] == 'X' ){
                return false;
            }
            index++;
        }
    } else if (dir == 'E'){
        int index = 1;
        while (index <= value){
            if (startC+index >= M || vec[startR][startC+index] == 'X' ){
                return false;
            }
            index++;
        }
    }
    return true;
}

vector<int> solution(vector<string> park, vector<string> routes) {
    N = park.size();
    M = park[0].length();
    
    for (int i = 0; i < N; i++){
        for (int j = 0; j < M; j++){
            if (park[i][j] == 'S'){
                startR = i;
                startC = j;
            }
        }
    }
    
    for (const string& route : routes){
        char dir = route[0];
        int value = route[2] - '0';
        
        bool isClear = move(dir, value, park);
        
        if (isClear){
            if (dir == 'N'){
                startR -= value;
            } else if (dir == 'S'){
                startR += value;
            } else if (dir == 'W'){
                startC -= value;
            } else if (dir == 'E'){
                startC += value;
            }
        }
    }
    
    return {startR, startC};
}