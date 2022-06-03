#include <iostream>
#include <algorithm>
using namespace std;
int main(){
  string s;cin>>s;
  sort(s.begin(),unique(s.begin(),s.end()),greater<char>());
  s.erase(unique(s.begin(),s.end()),s.end());
  int NS = 0,WE = 0;
  for(int i = 0; s.size() > i; i++){
    if(s[i] == 'N' || s[i] == 'S'){
      NS++;
    }
    if(s[i] == 'W' || s[i] == 'E'){
      WE++;
    }
  }
  if((NS == 0 || NS == 2) && (WE == 0 || WE == 2)){
    cout << "Yes" <<endl;
  }else {
    cout << "No" << endl;
  }
}