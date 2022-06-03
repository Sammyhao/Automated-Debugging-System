#include<bits/stdc++.h>
using namespace std;
int a[4];
char s[1010];
int get(char c)
{
	if (c=='N') {
		return 0; 
	}else if (c=='W') {
		return 1;
	}else if (c=='S') {
		return 2; 
	}else {
		return 3;
	}
}
int main()
{
	scanf("%s",s);
	int len=strlen(s);
	for (int i=0;i<len;i++) {
		a[get(s[i])]=1;
	}
	if (((a[0]+a[2])==0)&&((a[1]+a[3])%2==0)) {
		cout << "Yes" << endl;
	} else {
		cout << "No" << endl;
	}
}