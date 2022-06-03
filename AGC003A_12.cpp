#include <cstdio>
#include <cstring>
using namespace std;
char a[1005];
bool N=0,S=0,W=0,E=0;
int main (){
	scanf ("%s",a+1);int n=strlen(a+1);
    for (int i=1;i<=n;i++) {
        if (a[i]=='N') {
            N=1;
        }else if (a[i]=='S') {
            S=1;
        }else if (a[i]=='W') {
            W=1;
        }else {
            E=1;
        }
    }
    if ((N^S)||(W^E)) {
        puts("Yes");
    } else {
        puts("No");
    }
	return 0;
}



