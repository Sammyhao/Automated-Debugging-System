#include<cstdio>
#include<cstring>
#include<algorithm>
using namespace std;
const int N=1010;
int cnt[5];
char s[N];
int get(char s)
{
	if(s=='S') {
        return 1;
    }
    if(s=='W') {
        return 2;
    }
    if(s=='E') {
        return 3;
    }
    if(s=='N') {
        return 4;
    }
}
int main()
{
	scanf("%s",s);
	for(int i=0;s[i];i++) {
        cnt[get(s[i])]++;
    }
	if((cnt[1]+cnt[4]&&!cnt[1]*cnt[4])||(cnt[2]+cnt[3]&&!cnt[2]*cnt[3])) {
        puts("No");
    } else {
        puts("Yes");
    }
	return 0;
}