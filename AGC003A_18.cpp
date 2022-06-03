#include <iostream>
#include <cstring>
#include <cstdlib>
#include <stdio.h>
#include <algorithm>
#include <vector>
#include <queue>
#include <set>
#include <map>
#include <string>
#include <math.h>
#include <stdlib.h>
#include <iomanip>
#include <list>
#include <deque>
#include <stack>
#define ull unsigned long long
#define ll long long
#define mod 90001
#define INF 0x3f3f3f3f
#define maxn 10010
#define cle(a) memset(a,0,sizeof(a))
const ull inf = 1LL << 61;
const double eps=1e-5;
using namespace std;
priority_queue<int,vector<int>,greater<int> >pq;
struct Node{
	int x,y;
};
struct cmp{
    bool operator()(Node a,Node b){
        if(a.x==b.x) return a.y> b.y;
        return a.x>b.x;
	}
};

bool cmp(int a,int b){
    return a>b;
}
int main()
{
    #ifndef ONLINE_JUDGE
    #endif
    string s;
	cin>>s;
	int a=0,b=0;
	for(int i=0;i<s.size();i++){
		if(s[i]=='N') {
            a++;
        }else if(s[i]=='S'){
            a--;
        }if(s[i]=='E'){
            b++;
        }else if(s[i]=='W'){
            b--;
        }
	}
	if(a==0&&b==0){
		puts("Yes");
	}
	else {
        puts("No");
    }
    return 0;
}
