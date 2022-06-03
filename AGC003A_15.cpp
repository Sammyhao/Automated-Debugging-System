#include <bits/stdc++.h>
using namespace std;

int main() {
  int north=0, west=0;
  string s;
  cin >> s;
  int n = s.size();
  for (int i = 0; i < n; i++) {
    if (s.at(i) == 'N'){
      if (north < 2) north++;
    }
    else if (s.at(i)=='S') {
      if (north < 2) north++;
    }
    else if (s.at(i) == 'W') {
      if (west < 2) west++;
    }
    else if (s.at(i) == 'E') {
      if (west < 2) west++;
    }
  }
  if (north%2==0 && west%2==0) {
      cout << "Yes" << endl;
  } else {
      cout << "No" << endl;
  }
}


