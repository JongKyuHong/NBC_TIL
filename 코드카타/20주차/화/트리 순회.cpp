// https://www.acmicpc.net/problem/1991

#include <iostream>
#include <unordered_map>

using namespace std;

unordered_map<char, pair<char, char>> um;

void preorder(char node)
{
    cout << node;
    char left = um[node].first;
    if (left != '.')
    {
        preorder(left); 
    }
    char right = um[node].second;
    if (right != '.')
    {
        preorder(right);    
    }
}

void inorder(char node)
{
    char left = um[node].first;
    if (left != '.')
    {
        inorder(left);    
    }
    cout << node;
    char right = um[node].second;
    if (right != '.')
    {
        inorder(right);    
    }    
}

void postorder(char node)
{
    char left = um[node].first;
    if (left != '.')
    {
        postorder(left);    
    }
    char right = um[node].second;
    if (right != '.')
    {
        postorder(right);    
    }
    cout << node;
}


int main()
{
    ios_base::sync_with_stdio(false);
    cin.tie(0);
    
    int N;
    cin >> N;
    
    for (int i = 0; i < N; i++)
    {
        char node, left, right;
        cin >> node >> left >> right;
        
        um[node] = {left, right};
    }
    
    preorder('A');
    cout << "\n";
    inorder('A');
    cout << "\n";
    postorder('A');
    
    return 0;
}