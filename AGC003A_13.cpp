#include <iostream>
#include <vector>
#include <iomanip>
#include <string>
#include <sstream>
#include <algorithm>
#include <map>
#include <bits/stdc++.h>

using namespace std;
#define REP(i,n) FOR(i,0,n)
#define FOR(i,a,b) for(long long i=(a),i##Len_=(b);i<i##Len_;i++)
typedef long long ll;
static const ll MOD = 1000000007;
static const ll INF = 1000000000000000000LL;

//AGC003
//NがあったらSがないといけない
//WがあったらEがないといけない
int main() {
    string S;
    cin >> S;
    bool hasN = false;
    bool hasW = false;
    bool hasS = false;
    bool hasE = false;
    REP(i,S.size()) {
        if(S[i] == 'N') {
            hasN = true;
        }
        if(S[i] == 'W') {
            hasW = true;
        }
        if(S[i] == 'S') {
            hasS = true;
        }
        if(S[i] == 'E') {
            hasE = true;
        }
    }
    string res = "Yes";
    if(hasN && !hasS) {
        res = "No";
    }
    if(hasW && !hasE) {
        res = "No";
    }
    cout << res << endl;
}