#include <bits/stdc++.h>
using namespace std;

int main() {
  string S;
  cin >> S;
  bool n = S.find("N") >= 0;
  bool s = S.find("S") >= 0;
  bool e = S.find("E") >= 0;
  bool w = S.find("W") >= 0;
  if ((n && !s) || (!n && s) || (e && !w) || (!e && w)) {
    cout << "No" << endl;
  }
  else {
    cout << "Yes" << endl;
  }
}
