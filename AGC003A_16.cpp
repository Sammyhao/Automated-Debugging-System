//be naame khoda
#include<algorithm>
#include<iostream>
#include<cassert>
#include<iomanip>
#include<cstring>
#include<sstream>
#include<numeric>
#include<string>
#include<cstdio>
#include<vector>
#include<queue>
#include<stack>
#include<cmath>
#include<set>
#include<map> 

using namespace std;
 
typedef long long ll;
typedef pair<int, int> pii;

#define fir first
#define sec second
#define mp make_pair
#define pb push_back
#define sz(x) ((int)(x).size())

int main() {
    std::ios::sync_with_stdio(false);
    cin.tie(0);
    cout.tie(0);
   
    string s;
    cin >> s;
    int N = 0, W = 0, E = 0, S = 0;
    for (int i = 0; i < sz(s); i++) {
        if (s[i] == 'N') {
            N++;
        }
        if (s[i] == 'W') {
            W++;
        }
        if (s[i] == 'E') {
            E++;
        }
        if (s[i] == 'S') {
            S++;
        }
    }
    if (N == S && E == W) {
        cout << "Yes\n";
    } else {
        cout << "No\n";
    }
}
