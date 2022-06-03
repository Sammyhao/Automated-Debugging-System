#include<bits/stdc++.h>
using namespace std;
int main(){
	string a; cin>>a;
	int x=a.size();
	int s=0,e=0,n=0,w=0;
	for(int i=0;i<x;i++){
		if(a[i]=='S') {
            s++;
        }
        if(a[i]=='E') {
            e++;
        }
        if(a[i]=='N') {
            n++;
        }
        if(a[i]=='W') {
            w++;
        }
	}
	if((s>0&&n>0)||(w>0&&e>0)){
		cout<<"Yes"<<endl;
	}
	else{
		cout<<"No"<<endl;
	}
	return 0;
}