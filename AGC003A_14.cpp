#include <bits/stdc++.h>
using namespace std;
 
	int main() {
		string s;
		cin>>s;
		int a=0;
		int b=0;
		int c=0;
		int d=0;
		for(int i=0;i<s.size();i++){
			if(s.at(i)=='N') {
				a++;
            }
            if(s.at(i)=='S') {
				b++;
            }
            if(s.at(i)=='W') {
				c++;
            }
            if(s.at(i)=='E') {
				d++;
            }
		}
		if(a==b&&c==d) {
			cout<<"Yes";
        } else {
			cout<<"No";
        }
	}
