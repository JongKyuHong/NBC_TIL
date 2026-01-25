#include <iostream>
#include <string>
#include <algorithm>
#include <vector>

using namespace std;

string add(string a, string b) {
    string res = "";
    int carry = 0;
    int i = a.size() - 1, j = b.size() - 1;

    while (i >= 0 || j >= 0 || carry) {
        int sum = carry;
        if (i >= 0) sum += a[i--] - '0';
        if (j >= 0) sum += b[j--] - '0';
        res += (sum % 10) + '0';
        carry = sum / 10;
    }
    reverse(res.begin(), res.end());
    return res;
}

string subtract(string a, string b) {
    string res = "";
    int borrow = 0;
    int i = a.size() - 1, j = b.size() - 1;

    while (i >= 0) {
        int sub = (a[i--] - '0') - borrow - (j >= 0 ? b[j--] - '0' : 0);
        if (sub < 0) {
            sub += 10;
            borrow = 1;
        } else {
            borrow = 0;
        }
        res += sub + '0';
    }
    while (res.size() > 1 && res.back() == '0') res.pop_back();
    reverse(res.begin(), res.end());
    return res;
}

bool isGreater(string a, string b) {
    if (a.size() != b.size()) return a.size() > b.size();
    return a > b;
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    string s1, s2;
    cin >> s1 >> s2;

    bool neg1 = (s1[0] == '-');
    bool neg2 = (s2[0] == '-');

    if (neg1) s1 = s1.substr(1);
    if (neg2) s2 = s2.substr(1);

    if (neg1 == neg2) {
        if (neg1) cout << "-";
        cout << add(s1, s2) << endl;
    } else {
        if (s1 == s2) {
            cout << 0 << endl;
        } else if (isGreater(s1, s2)) {
            if (neg1) cout << "-";
            cout << subtract(s1, s2) << endl;
        } else {
            if (neg2) cout << "-";
            cout << subtract(s2, s1) << endl;
        }
    }

    return 0;
}