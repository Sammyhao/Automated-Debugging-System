#include <bits/stdc++.h>
using namespace std;
int main () {
  string T;
  cin >> T;
  int N = 0, E = 0, S = 0, W = 0;
  for(int i = 0; i < T.size() ;i++){
    if(T.at(i) == 'N') {
        N++;
    }else if(T.at(i) == 'E') {
        E++;
    }else if(T.at(i) == 'S') {
        S++;
    }else if(T.at(i) == 'W') {
        W++;
    }
  }
  int flag = 1;
  if(N == 0 && S != 0) {
      flag = 0;
  }
  if(N != 0 && S == 0) {
      flag = 0;
  }
  if(W == 0 && E != 0) {
      flag = 0;
  }
  if(W != 0 && E != 0) {
      flag = 0;
  }
  cout << (flag?"Yes":"No") << endl;
  return 0;
}
