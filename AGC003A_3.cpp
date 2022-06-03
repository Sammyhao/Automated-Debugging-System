#include <bits/stdc++.h>
using namespace std;
const char Key[4] = {'N','S','W','E'};

int main(){
    vector<bool> res(2,true);
    string Str; cin >> Str;
    int N = Str.size();
    multiset<char> S;
    for (int i = 0; i < N; ++i){
        S.insert(Str[i]);
    }
    for (int j = 0; j < 2; ++j){
        if (S.count(Key[j]) != S.count(Key[j + 1])){
            if (min(S.count(Key[j]),S.count(Key[j + 1])) == 0) {
                res[j] = false;
            }
        }
    }
    if (res[0] && res[1]) {
        cout << "Yes" << endl;
    } else {
        cout << "No" << endl;
    }
}