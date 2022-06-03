// 簡易競プロテンプレ
#include <bits/stdc++.h>
using namespace std;

int c[4];
int main(void){
 string s; cin >> s;
 for(int i=0;i<s.size();i++){
  if (s[i]=='N') {
      c[0]++;
  }
  if (s[i]=='S') {
      c[1]++;
  }
  if (s[i]=='E') {
      c[2]++;
  }
  if (s[i]=='W') {
      c[3]++;
  }
 }
 if (((int)(c[0]>0))^((int)(c[1]>0)) || ((int)(c[0]>0))^((int)(c[1]>0))){
    cout << "No" << endl;
 }else {
    cout << "Yes" << endl;
 }
 return 0;
}