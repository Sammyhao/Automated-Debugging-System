#include <bits/stdc++.h>

using namespace std;

int main(){
    string s;
    cin >> s;
    int Up = 0,Left = 0;
    for(int i = 0; i < s.length(); i++){
        if(s[i]=='N'){
            Up++;
        }else if(s[i]=='S'){
            Up--;
        }else if(s[i]=='E'){
            Left++;
        }else{ 
            Left--;
        }
    }
    cout << ((Up==0&&Left==0)?"Yes":"No");
}
