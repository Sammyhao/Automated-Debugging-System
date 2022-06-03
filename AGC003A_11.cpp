#include<iostream>
using namespace std;
string s;
int a[4];
int main(){
    cin>>s;
    for(int i=0;i<s.size();i++){
        if(s[i]=='N'){
            a[0]++;
        }
        if(s[i]=='W'){
            a[1]++;
        }
        if(s[i]=='S'){
            a[2]++;
        }
        if(s[i]=='E'){
            a[3]++;
        }
    }
    int cnt=0;
    for(int i=0;i<4;i++){
        if(a[i]){
            cnt++;
        }
    }
    cout<<(cnt%2==0 ? "Yes":"No")<<endl;
}