#include <bits/stdc++.h>
using namespace std;

int main() {
	string str;
	cin >> str;
	if(str.find('N') == string::npos){
		if(str.find('S') != string::npos){
			cout << "No" << endl;
			return 0;
		}
	}
	else
		if(str.find('S') == string::npos){
			cout << "No" << endl;
			return 0;
		}
		
	if(str.find('W') == string::npos){
		if(str.find('E') != string::npos){
			cout << "No" << endl;
			return 0;
		}
	}
	else
		if(str.find('E') == string::npos){
			cout << "No2" << endl;
			return 0;
		}
		
	cout << "Yes" << endl;	
	return 0;
}